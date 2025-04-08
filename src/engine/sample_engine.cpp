#include "sample_engine.h"

#include "framework/nodes/environment_3d.h"
#include "framework/parsers/parse_gltf.h"
//#include "framework/parsers/parse_obj.h"
#include "framework/input.h"
#include "framework/ui/io.h"

#include "graphics/sample_renderer.h"
#include "graphics/renderer_storage.h"

#include "engine/scene.h"

#include "shaders/mesh_forward.wgsl.gen.h"
#include "shaders/mesh_grid.wgsl.gen.h"

#include "spdlog/spdlog.h"

#include "../graph/graph.h"
#include "../graph/print_node.h"
#include "../graph/sequence_node.h"
#include "../graph/start_node.h"
#include "../graph/event_node.h"
#include "../graph/graph_button_2d.h"
#include <graph/graph_node3D.h>
#include <graph/rotate_node.h>
#include "framework/nodes/panel_2d.h"
#include <backends/imgui_impl_glfw.h>
#include "framework/utils/ImGuizmo.h"


int SampleEngine::initialize(Renderer* renderer, sEngineConfiguration configuration)
{
	return Engine::initialize(renderer, configuration);
}

void SampleEngine::setupGraphUI() {
    if (!main_scene) {
        std::cerr << "Cannot create UI - no main scene\n";
        return;
    }

    // Create UI panel
    ui::Panel2D* ui_panel = new ui::Panel2D("GraphPanel", glm::vec2(20, 20), glm::vec2(250, 120), 0u, colors::GRAY);
    main_scene->add_node(ui_panel);

    // Setup execute button
    ui::sButtonDescription buttonDesc;
    buttonDesc.label = "Run Graph";
    buttonDesc.size = glm::vec2(200, 50);
    buttonDesc.color = colors::WHITE;
    buttonDesc.position = glm::vec2(25, 35);

    // Find event graph
    GraphSystem::Graph* eventGraph = nullptr;
    for (auto graph : graphManager.getGraphs()) {
        if (!graph->isTickGraph()) {
            eventGraph = graph;
            break;
        }
    }

    if (eventGraph) {
        GraphSystem::GraphButton2D* executeBtn = new GraphSystem::GraphButton2D("ExecuteBtn", buttonDesc, eventGraph);
        ui_panel->add_child(executeBtn);
    }
}

void buildPipeline(GraphSystem::Graph& graph) {
    // Create and register nodes
    auto* eventNode = new GraphSystem::EventNode("ButtonEvent");
    graph.addNode(eventNode);  // Explicitly add to graph first

    auto* sequenceNode = new GraphSystem::SequenceNode("MainSequence", 2);
    auto* printA = new GraphSystem::PrintNode("DebugPrintA");
    auto* printB = new GraphSystem::PrintNode("DebugPrintB");

    // Add remaining nodes to graph
    graph.addNode(sequenceNode);
    graph.addNode(printA);
    graph.addNode(printB);

    // Set messages
    printA->getInput("Message")->setData<std::string>("Hello from Node A");
    printB->getInput("Message")->setData<std::string>("Hello from Node B");

    // Connect nodes
    graph.connect(eventNode, "Execution", sequenceNode, "Execute");
    graph.connect(sequenceNode, "Step1", printA, "Execute");
    graph.connect(sequenceNode, "Step2", printB, "Execute");

    // Mark as entry point
    eventNode->setEntryPoint(true);
}

int SampleEngine::post_initialize()
{
    Engine::post_initialize();

    main_scene = new Scene("main_scene");

    // Create skybox
    {
        MeshInstance3D* skybox = new Environment3D();
        main_scene->add_node(skybox);
    }

    // Load Meta Quest Controllers and Controller pointer
    if (renderer->get_xr_available())
    {
        std::vector<Node*> entities_left;
        std::vector<Node*> entities_right;
        GltfParser parser;
        parser.parse("data/meshes/controllers/left_controller.glb", entities_left);
        parser.parse("data/meshes/controllers/right_controller.glb", entities_right);
        controller_mesh_left = static_cast<Node3D*>(entities_left[0]);
        controller_mesh_right = static_cast<Node3D*>(entities_right[0]);
    }

    // Create grid
    {
        MeshInstance3D* grid = new MeshInstance3D();
        grid->set_name("Grid");
        grid->add_surface(RendererStorage::get_surface("quad"));
        grid->set_position(glm::vec3(0.0f));
        grid->rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        grid->scale(glm::vec3(10.f));
        grid->set_frustum_culling_enabled(false);

        // NOTE: first set the transparency and all types BEFORE loading the shader
        Material* grid_material = new Material();
        grid_material->set_transparency_type(ALPHA_BLEND);
        grid_material->set_cull_type(CULL_NONE);
        grid_material->set_type(MATERIAL_UNLIT);
        grid_material->set_shader(RendererStorage::get_shader_from_source(shaders::mesh_grid::source, shaders::mesh_grid::path, shaders::mesh_grid::libraries, grid_material));
        grid->set_surface_material_override(grid->get_surface(0), grid_material);

        main_scene->add_node(grid);
    }


    //Setup Graph EventNode and SequenceNode. 
    GraphSystem::Graph* eventGraph = graphManager.createGraph("MainGraph", false);
    auto* eventNode = new GraphSystem::EventNode("ButtonEvent");
    eventGraph->addNode(eventNode);
    auto* sequenceNode = new GraphSystem::SequenceNode("MainSequence", 1);
    eventGraph->addNode(sequenceNode);

    // buildPipeline(*eventGraph);
    

    // Create test box
    MeshInstance3D* testBox = new MeshInstance3D();
    testBox->set_name("TestBox");
    testBox->set_position(glm::vec3(0.0f, 1.0f, 0.0f));
    testBox->scale(glm::vec3(1.0f));

    // Get surface and validate
    Surface* boxSurface = RendererStorage::get_surface("box");
    testBox->add_surface(boxSurface);

    // Create material for the box
    Material* box_mat = new Material();
    box_mat->set_transparency_type(ALPHA_OPAQUE);
    box_mat->set_cull_type(CULL_BACK);
    box_mat->set_type(MATERIAL_PBR);
    box_mat->set_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    // Get shader
    Shader* boxShader = RendererStorage::get_shader_from_source(
        shaders::mesh_forward::source,
        shaders::mesh_forward::path,
        shaders::mesh_forward::libraries,
        box_mat
    );
    box_mat->set_shader(boxShader);
    testBox->set_surface_material_override(testBox->get_surface(0), box_mat);

    main_scene->add_node(testBox);

    // Create and setup graph nodes
    GraphNode3D* boxNode = new GraphNode3D("BoxNode", eventGraph);
    boxNode->setTransform(testBox->get_transform());
    eventGraph->addNode(boxNode);

    GraphSystem::RotateNode* rotator = new GraphSystem::RotateNode("BoxRotator", 30.0f, glm::vec3(0, 1, 0));
    rotator->setTarget(testBox);
    eventGraph->addNode(rotator);

    // Connect the rotator to the box node
    if (!eventGraph->connect(rotator, "Transform", boxNode, "Transform")) {
        std::cerr << "Failed to connect RotateNode to BoxNode\n";
    }

    rotator->setExecutionPending(true);
 

    // Connect nodes
    eventGraph->connect(eventNode, "Execution", sequenceNode, "Execute");
    eventGraph->connect(sequenceNode, "Step1", rotator);
    
    eventNode->setExecutionPending(true);

    setupGraphUI();

    return 0u;
}

void SampleEngine::on_frame() {
    Engine::on_frame();
}


void SampleEngine::clean()
{
    Engine::clean();
}

void SampleEngine::update(float delta_time)
{
    Engine::update(delta_time);

    main_scene->update(delta_time);

    if (renderer->get_xr_available()) {
        controller_mesh_left->set_transform(Transform::mat4_to_transform(Input::get_controller_pose(HAND_LEFT)));
        controller_mesh_right->set_transform(Transform::mat4_to_transform(Input::get_controller_pose(HAND_RIGHT)));
    }
}

void SampleEngine::render()
{
    if (show_imgui) {
        render_default_gui();
    }

    main_scene->render();

    if (renderer->get_xr_available()) {
        controller_mesh_left->render();
        controller_mesh_right->render();
    }

    Engine::render();
}
