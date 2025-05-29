#include "sample_engine.h"

#include "framework/nodes/environment_3d.h"
#include "framework/parsers/parse_gltf.h"
#include "framework/parsers/parse_scene.h"
#include "framework/input.h"
#include "framework/math/intersections.h"
#include "framework/ui/io.h"

#include "graphics/sample_renderer.h"
#include "graphics/renderer_storage.h"

#include "engine/scene.h"

#include "shaders/mesh_forward.wgsl.gen.h"
#include "shaders/mesh_grid.wgsl.gen.h"
#include "shaders/ui/ui_ray_pointer.wgsl.gen.h"

#include "spdlog/spdlog.h"

#include "io.h"
#include "../graph/graph.h"
#include "../graph/print_node.h"
#include "../graph/sequence_node.h"
#include "../graph/start_node.h"
#include "../graph/run_node.h"
#include "../graph/graph_button_2d.h"
#include "graph/node_widget_2d.h" 
#include <graph/graph_node3D.h>
#include <graph/rotate_node.h>
#include "framework/nodes/panel_2d.h"
#include <backends/imgui_impl_glfw.h>
#include "framework/utils/ImGuizmo.h"

#include "graph/context_menu.h"


int SampleEngine::initialize(Renderer* renderer, const sEngineConfiguration& configuration)
{
    return Engine::initialize(renderer, configuration);
}

void SampleEngine::push_context_menu(ui::ContextMenu* cm) {
    active_context_menu = cm;

    if (graph_container) {
        graph_container->add_child(cm);  
    }
}

void SampleEngine::delete_context_menu(ui::ContextMenu* cm) {
    if (active_context_menu == cm) {
        active_context_menu = nullptr;
    }

    IO::blur();  

    if (graph_container) {
        graph_container->remove_child(cm);  
    }

    delete cm;  
}





void SampleEngine::setupGraphUI() {
    if (!main_scene) {
        std::cerr << "Cannot create UI - no main scene\n";
        return;
    }

    // Create UI panel
    run_panel = new ui::Panel2D("RunPanel", glm::vec2(20, 20), glm::vec2(250, 120), ui::CREATE_3D, colors::GRAY);
    main_scene->add_node(run_panel);

    // Setup execute button
    ui::sButtonDescription buttonDesc;
    buttonDesc.label = "Run Graph";
    buttonDesc.size = glm::vec2(200, 50);
    buttonDesc.color = colors::WHITE;
    buttonDesc.position = glm::vec2(25, 35);

    // Find event graph
    GraphSystem::Graph* eventGraph = nullptr;
    eventGraph = graphManager.getGraphs()[0];

    if (eventGraph) {
        GraphSystem::GraphButton2D* executeBtn = new GraphSystem::GraphButton2D("ExecuteBtn", buttonDesc, eventGraph);
        run_panel->add_child(executeBtn);
    }


}

void buildPipeline(GraphSystem::Graph& graph) {
    // Create and register nodes
    auto* eventNode = new GraphSystem::RunNode("Begin Play");
    graph.addNode(eventNode);

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
    using namespace ui;

    constexpr float X = 20.0f;
    constexpr float Y = 400.0f;
    constexpr float PANEL_W = 260.0f;


    VContainer2D* panel = new VContainer2D(
        "NodeCreationPanel",
        { X, Y },
        0u,
        colors::GREEN
    );

    // Padding interior y separación entre filas
    panel->padding = { 12.0f, 12.0f };
    panel->item_margin = { 8.0f, 8.0f };

    // Fijamos sólo el ancho;
    panel->set_fixed_size({ PANEL_W, 0.0f });
    panel->use_fixed_size = true;

    // Añadimos el título
    {
        Text2D* title = new Text2D("Create Node", { 0, 0 }, 20.0f, TEXT_CENTERED);
        title->set_color(colors::BLACK);
        panel->add_child(title);
    }

    // Fila (label + botón)
    auto addRow = [&](const std::string& name, const std::string& type) {
        HContainer2D* row = new HContainer2D(
            "Row_" + name,
            { 0, 0 },
            0u,
            colors::GREEN
        );
        row->padding = { 4.0f, 2.0f };
        row->item_margin = { 8.0f, 0.0f };
        panel->add_child(row);

        // etiqueta
        Text2D* lbl = new Text2D(name, { 0, 0 }, 16.0f, TEXT_CENTERED);
        lbl->set_color(colors::WHITE);
        row->add_child(lbl);

        // botón
        sButtonDescription desc;
        desc.label = "";
        desc.size = { 24.0f, 24.0f };
        desc.color = colors::WHITE;
        auto* btn = new GraphSystem::GraphButton2D(
            name + "Btn",
            desc,
            nullptr,
            editor,
            type
        );
        row->add_child(btn);
        };

    // Creamos las filas
    addRow("PrintNode", "PrintNode");
    addRow("RotateNode", "RotateNode");
    addRow("SequenceNode", "SequenceNode");
    addRow("RunNode", "RunNode");
    addRow("GraphNode3D", "GraphNode3D");
    addRow("MathNode", "MathNode");
    addRow("VariableNode", "VariableNode");
    addRow("BranchNode", "BranchNode");
    addRow("TickNode", "TickNode");
    addRow("ScaleNode", "ScaleNode");
    addRow("EntityNode3D", "EntityNode3D");


    panel->use_fixed_size = false;
    panel->on_children_changed();

    main_scene->add_node(panel);
}


int SampleEngine::post_initialize()
{
    Engine::post_initialize();

    main_scene = new Scene("main_scene");

    graph_container = new Node2D(
        "GraphContainer",       
        { 0,0 },
        { 0,0 },   
        ui::CREATE_3D          
    );

    main_scene->add_node(graph_container);

    // skybox
    {
        MeshInstance3D* skybox = new Environment3D();
        main_scene->add_node(skybox);
    }

    // Load Meta Quest Controllers and Controller pointer
    if (renderer->get_xr_available())
    {
        std::vector<Node*> left, right;
        GltfParser parser;
        parser.parse("data/meshes/controllers/left_controller.glb", left);
        parser.parse("data/meshes/controllers/right_controller.glb", right);
        controller_mesh_left = static_cast<Node3D*>(left[0]);
        controller_mesh_right = static_cast<Node3D*>(right[0]);

        // Controller pointer
        ray_pointer = parse_mesh("data/meshes/raycast.obj");

        Material* pointer_material = new Material();
        pointer_material->set_transparency_type(ALPHA_BLEND);
        pointer_material->set_cull_type(CULL_NONE);
        pointer_material->set_type(MATERIAL_UNLIT);
        pointer_material->set_shader(RendererStorage::get_shader_from_source(shaders::ui_ray_pointer::source, shaders::ui_ray_pointer::path, shaders::ui_ray_pointer::libraries, pointer_material));

        ray_pointer->set_surface_material_override(ray_pointer->get_surface(0), pointer_material);

        sphere_pointer = parse_mesh("data/meshes/sphere.obj");

        Material* sphere_pointer_material = new Material();
        sphere_pointer_material->set_depth_read(false);
        sphere_pointer_material->set_priority(0);
        sphere_pointer_material->set_transparency_type(ALPHA_BLEND);
        sphere_pointer_material->set_type(MATERIAL_UNLIT);
        sphere_pointer_material->set_shader(RendererStorage::get_shader_from_source(shaders::mesh_forward::source, shaders::mesh_forward::path, shaders::mesh_forward::libraries, sphere_pointer_material));

        sphere_pointer->set_surface_material_override(sphere_pointer->get_surface(0), sphere_pointer_material);
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
    GraphSystem::Graph* eventGraph = graphManager.createGraph("MainGraph");

    editor = new GraphSystem::GraphEditor(eventGraph,graph_container);


    // instantiate logical nodes
    auto* runNode = static_cast<GraphSystem::RunNode*>(
        editor->createNode("RunNode", "Begin Play", { 550.0f, 550, 0.0f })
        );
    runNode->setEntryPoint(true);

    auto* sequenceNode = static_cast<GraphSystem::SequenceNode*>(
        editor->createNode("SequenceNode", "MainSequence", { 600.0f, 600.0f, 0.0f })
        );

    // red test-box in world
    MeshInstance3D* testBox = new MeshInstance3D();
    testBox->set_name("TestBox");
    testBox->set_position({ 0,1,0 });
    testBox->scale({ 1,1,1 });
    testBox->add_surface(RendererStorage::get_surface("box"));
    testBox->update_aabb();

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


    auto* boxNode = static_cast<GraphSystem::GraphNode3D*>(
        editor->createNode("GraphNode3D", "BoxNode", { 300.0f, 300.0f, 0.0f })
        );
    boxNode->setTransform(testBox->get_transform());

    auto* rotator = static_cast<GraphSystem::RotateNode*>(
        editor->createNode("RotateNode", "BoxRotator", { 150.0f, 150.0f, 0.0f })
        );
    rotator->setTarget(testBox);
    rotator->setRotationAngle(30.0f);
    rotator->setRotationAxis({ 0,1,0 });


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
    if (renderer->get_xr_available()) {
        controller_mesh_right->set_transform(Transform::mat4_to_transform(Input::get_controller_pose(HAND_RIGHT)));
        controller_mesh_left->set_transform(Transform::mat4_to_transform(Input::get_controller_pose(HAND_LEFT)));
    }

    Engine::update(delta_time);
    for (auto* graph : graphManager.getGraphs()) {
        graph->update(delta_time);
    }
    
    if (renderer->get_xr_available()) {
        glm::mat4 pose = Input::get_controller_pose(HAND_LEFT, POSE_AIM);
        pose = glm::translate(pose, glm::vec3(0.0f, 0.05f, -0.04f));
        pose = glm::rotate(pose, glm::radians(120.f), glm::vec3(1, 0, 0));

        glm::mat4 poseR = Input::get_controller_pose(HAND_RIGHT, POSE_AIM);
        poseR = glm::translate(poseR, glm::vec3(0.0f, 0.05f, -0.04f));
        poseR = glm::rotate(poseR, glm::radians(120.f), glm::vec3(1, 0, 0));

        if (graph_container) {
            graph_container->set_xr_transform(Transform::mat4_to_transform(poseR));
            graph_container->update(delta_time);
        }

        if (run_panel) {

            run_panel->set_xr_transform(Transform::mat4_to_transform(pose));
            run_panel->update(delta_time);
        }
       
    }

    bool select_pressed = Input::was_trigger_pressed(HAND_RIGHT) || Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    if (select_pressed) {

        const std::vector<std::string>& notAllowedNames = { "Environment3D",  "Grid" };

        for(auto node : main_scene->get_nodes())
        {
            auto node3d = dynamic_cast<Node3D*>(node);
            if (!node3d) continue;
            if (std::find(notAllowedNames.begin(), notAllowedNames.end(), node3d->get_name()) != notAllowedNames.end()) {
                continue;
            }

            glm::vec3 ray_origin;
            glm::vec3 ray_direction;
            float distance = 1e9f;

            Engine::instance->get_scene_ray(ray_origin, ray_direction);

            if (node3d->test_ray_collision(ray_origin, ray_direction, distance)) {

                // If needed:
                // const glm::vec3& intersection_point = ray_direction + ray_direction * distance;
            }
        }

    }

    main_scene->update(delta_time);
}

void SampleEngine::render()
{
    if (show_imgui) {
        render_default_gui();
    }

    main_scene->render();

    if (renderer->get_xr_available()) {

        const glm::mat4x4& raycast_transform = Input::get_controller_pose(HAND_RIGHT, POSE_AIM);
        ray_pointer->set_transform(Transform::mat4_to_transform(raycast_transform));
        ray_pointer->scale(glm::vec3(1.0f, 1.0f, 0.5f));
        ray_pointer->render();

        sphere_pointer->set_transform(Transform::mat4_to_transform(raycast_transform));
        sphere_pointer->scale(glm::vec3(0.1f));
        sphere_pointer->render();

        controller_mesh_left->render();
        controller_mesh_right->render();
    }

    Engine::render();
}


void SampleEngine::append_glb(const std::string& filename)
{
    std::vector<Node*> entities;

    GltfParser gltf_parser;

    gltf_parser.parse(filename.c_str(), entities, PARSE_NO_FLAGS);

    if (!entities.empty()) {
        main_scene->add_nodes(entities);
    }
}
