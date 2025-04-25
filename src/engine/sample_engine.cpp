#include "sample_engine.h"

#include "framework/nodes/environment_3d.h"
#include "framework/parsers/parse_gltf.h"
//#include "framework/parsers/parse_obj.h"
#include "framework/input.h"
#include "framework/math/intersections.h"
#include "framework/ui/io.h"

#include "graphics/sample_renderer.h"
#include "graphics/renderer_storage.h"

#include "engine/scene.h"

#include "shaders/mesh_forward.wgsl.gen.h"
#include "shaders/mesh_grid.wgsl.gen.h"

#include "spdlog/spdlog.h"

#include "io.h"
#include "../graph/graph.h"
#include "../graph/print_node.h"
#include "../graph/sequence_node.h"
#include "../graph/start_node.h"
#include "../graph/event_node.h"
#include "../graph/graph_button_2d.h"
#include "graph/node_widget_2d.h" 
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



void SampleEngine::setupNodeCreationUI(GraphSystem::GraphEditor* editor) {

    ui::Panel2D* nodePanel = new ui::Panel2D(
        "NodeCreationPanel",
        glm::vec2(20, 20),          
        glm::vec2(250, 300),         
        0u,                         
        colors::GREEN
    );

    nodePanel->set_position(glm::vec3(0.0f, 450.0f, 1.0f));

    int yOffset = 20;
        auto addCreationButton = [&](const std::string& label, const std::string& nodeType) {
            ui::sButtonDescription desc;
            desc.label = "Add " + label;
            desc.size = glm::vec2(200, 40);
            desc.color = colors::WHITE;
            desc.position = glm::vec2(25, yOffset);
    
            
            GraphSystem::GraphButton2D* button = new GraphSystem::GraphButton2D(
                label + "Btn",
                desc,
                nullptr, 
                editor,
                nodeType
            );
    
            nodePanel->add_child(button);
            yOffset += 50;
            };

        //Create visual Nodes
        addCreationButton("PrintNode", "PrintNode");
        addCreationButton("RotateNode", "RotateNode");
        addCreationButton("SequenceNode", "SequenceNode");
        addCreationButton("EventNode", "EventNode");
        addCreationButton("GraphNode3D", "GraphNode3D");

    main_scene->add_node(nodePanel);

}

int SampleEngine::post_initialize()
{
    Engine::post_initialize();

    main_scene = new Scene("main_scene");

    // skybox
    {
        MeshInstance3D* skybox = new Environment3D();
        main_scene->add_node(skybox);
    }

    // load controllers if VR
    if (renderer->get_xr_available())
    {
        std::vector<Node*> left, right;
        GltfParser parser;
        parser.parse("data/meshes/controllers/left_controller.glb", left);
        parser.parse("data/meshes/controllers/right_controller.glb", right);
        controller_mesh_left = static_cast<Node3D*>(left[0]);
        controller_mesh_right = static_cast<Node3D*>(right[0]);
    }

    // grid
    {
        MeshInstance3D* grid = new MeshInstance3D();
        grid->set_name("Grid");
        grid->add_surface(RendererStorage::get_surface("quad"));
        grid->set_position({ 0,0,0 });
        grid->rotate(glm::radians(90.0f), { 1,0,0 });
        grid->scale({ 10,10,10 });
        grid->set_frustum_culling_enabled(false);

        Material* mat = new Material();
        mat->set_transparency_type(ALPHA_BLEND);
        mat->set_cull_type(CULL_NONE);
        mat->set_type(MATERIAL_UNLIT);
        mat->set_shader(RendererStorage::get_shader_from_source(
            shaders::mesh_grid::source,
            shaders::mesh_grid::path,
            shaders::mesh_grid::libraries,
            mat
        ));
        grid->set_surface_material_override(grid->get_surface(0), mat);
        main_scene->add_node(grid);
    }

    // create graph & editor
    GraphSystem::Graph* eventGraph = graphManager.createGraph("MainGraph", false);
    editor = new GraphSystem::GraphEditor(eventGraph);


    // instantiate logical nodes
    auto* eventNode = static_cast<GraphSystem::EventNode*>(
        editor->createNode("EventNode", "ButtonEvent", { -550.0f, 550, 0.0f })
        );
    eventNode->setEntryPoint(true);

    auto* sequenceNode = static_cast<GraphSystem::SequenceNode*>(
        editor->createNode("SequenceNode", "MainSequence", { 600.0f, 600.0f, 0.0f })
        );

    // red test-box in world
    MeshInstance3D* testBox = new MeshInstance3D();
    testBox->set_name("TestBox");
    testBox->set_position({ 0,1,0 });
    testBox->scale({ 1,1,1 });
    testBox->add_surface(RendererStorage::get_surface("box"));
    {
        Material* boxMat = new Material();
        boxMat->set_transparency_type(ALPHA_OPAQUE);
        boxMat->set_cull_type(CULL_BACK);
        boxMat->set_type(MATERIAL_PBR);
        boxMat->set_color({ 1,0,0,1 });
        boxMat->set_shader(RendererStorage::get_shader_from_source(
            shaders::mesh_forward::source,
            shaders::mesh_forward::path,
            shaders::mesh_forward::libraries,
            boxMat
        ));
        testBox->set_surface_material_override(testBox->get_surface(0), boxMat);
    }
    main_scene->add_node(testBox);

    // graph nodes at fixed height
    auto* boxNode = static_cast<GraphSystem::GraphNode3D*>(
        editor->createNode("GraphNode3D", "BoxNode", { 300.0f, 300.0f, 0.0f })
        );
    boxNode->setTransform(testBox->get_transform());

    auto* rotator = static_cast<GraphSystem::RotateNode*>(
        editor->createNode("RotateNode", "BoxRotator", { 1.0f, 150.0f, 0.0f })
        );
    rotator->setTarget(testBox);
    rotator->setRotationAngle(30.0f);
    rotator->setRotationAxis({ 0,1,0 });

    // connect and trigger
    eventGraph->connect(eventNode, "Execution", sequenceNode, "Execute");
    eventGraph->connect(sequenceNode, "Step1", rotator, "Execute");
    eventGraph->connect(rotator, "Transform", boxNode, "Transform");
    eventNode->setExecutionPending(true);

    setupGraphUI();
    setupNodeCreationUI(editor);

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
