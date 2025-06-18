#include "sample_engine.h"

#include "framework/nodes/environment_3d.h"
#include "framework/parsers/parse_gltf.h"
#include "framework/parsers/parse_scene.h"
#include "framework/input.h"
#include "framework/math/intersections.h"
#include "framework/utils/tinyfiledialogs.h"
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
#include <graph/rotate_node.h>
#include "framework/nodes/panel_2d.h"
#include <backends/imgui_impl_glfw.h>
#include "framework/utils/ImGuizmo.h"
#include "graph/graph_node.h"
#include "graph/entity_node_3d.h"

#include "graph/context_menu.h"

#include "graph/keyboard.h"
#include <graph/variable_node.h>
#include <graph/math_node.h>
#include <graph/tick_node.h>
#include <graph/set_variable_node.h>
#include "graph/create_node_button.h"
#include <iostream>




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

    run_panel = new ui::Panel2D("RunPanel", glm::vec2(20, 20), glm::vec2(84, 84), ui::CREATE_3D);
    run_panel->render_background = false; 
    main_scene->add_node(run_panel);

    GraphSystem::Graph* eventGraph = nullptr;
    if (graphManager.getGraphs().empty()) {
        return;
    }
    eventGraph = graphManager.getGraphs()[0];

    ui::sButtonDescription buttonDesc;
    buttonDesc.position = { 10.0f, 10.0f };   
    buttonDesc.size = { 64.0f, 64.0f };       
    buttonDesc.label = "";                    

    buttonDesc.path = "data/textures/play_icon.png";
    auto* executeBtn = new GraphSystem::GraphButton2D("Run", buttonDesc, eventGraph);

    run_panel->add_child(executeBtn);
}

void buildPipeline(GraphSystem::Graph& graph) {
    // Create and register nodes
    auto* eventNode = new GraphSystem::RunNode("Begin Play");
    graph.addNode(eventNode);

   // auto* sequenceNode = new GraphSystem::SequenceNode("MainSequence", 2);
    auto* printA = new GraphSystem::PrintNode("DebugPrintA");
    auto* printB = new GraphSystem::PrintNode("DebugPrintB");

    // Add remaining nodes to graph
    //graph.addNode(sequenceNode);
    graph.addNode(printA);
    graph.addNode(printB);

    // Set messages
    printA->getInput("Message")->setData(GraphSystem::VariableValue(std::string("Hello from Node A")));
    printB->getInput("Message")->setData(GraphSystem::VariableValue(std::string("Hello from Node B")));

    // Connect nodes
    //graph.connect(eventNode, "Execution", sequenceNode, "Execute");
   //  graph.connect(sequenceNode, "Step1", printA, "Execute");
   //    graph.connect(sequenceNode, "Step2", printB, "Execute");

    // Mark as entry point
    eventNode->setEntryPoint(true);
}


void SampleEngine::setupNodeCreationUI(GraphSystem::GraphEditor* editor) {
    using namespace ui;
    using namespace GraphSystem;
    
    constexpr float X = 0.0f;
    constexpr float Y = 84.0f;
    constexpr float PANEL_W = 260.0f;

    VContainer2D* panel = new VContainer2D(
        "NodeCreationPanel",
        { X, Y }, 
        0u,
        Color(0.0f, 0.0f, 0.0f, 0.0f)
    );
    
    panel->padding = { 8.0f, 8.0f };
    panel->item_margin = { 4.0f, 4.0f };
    panel->set_fixed_size({ PANEL_W, 0.0f });
    run_panel->add_child(panel);

    auto addCategoryTitle = [&](const std::string& title, NodeCategory category) {
        auto* title_panel = new XRPanel(
            "TitlePanel_" + title,
            { 0, 0 },
            { PANEL_W, 26.0f },
            0u,
            GraphSystem::NodeWidget2D::getColorForCategory(category)
        );

        auto* title_text = new Text2D(title, { 0, 0 }, 18.0f, TEXT_CENTERED | SKIP_TEXT_RECT);
        title_text->set_color(colors::WHITE);
        title_panel->add_child(title_text);

        panel->add_child(title_panel);
        };

    auto addRow = [&](const std::string& name, const std::string& type) {
        auto* row = new HContainer2D(
            "Row_" + name,
            { 0, 0 },
            0u,
            Color(0.2f, 0.2f, 0.2f, 0.5f)
        );
        
        row->padding = { 4.0f, 1.0f };
        row->item_margin = { 6.0f, 0.0f };
        panel->add_child(row);

        
        auto* lbl = new Text2D(name, { 0, 0 }, 16.0f, TEXT_CENTERED);
        lbl->set_color(colors::BLACK);
        row->add_child(lbl);

        sButtonDescription desc;
        desc.label = "";
       
        desc.size = { 22.0f, 22.0f };
        desc.path = "data/textures/submenu_mark.png";
        auto* btn = new GraphSystem::CreateNodeButton(
            "Create",
            desc,
            editor,
            type
        );
        row->add_child(btn);
        };

    addCategoryTitle("Flow Control", NodeCategory::FLOW);
    addRow("RunNode", "RunNode");
    addRow("BranchNode", "BranchNode");
    addRow("TickNode", "TickNode");
    addRow("LoopNode", "LoopNode");
    addRow("TimerNode", "TimerNode");

    addCategoryTitle("Data", NodeCategory::DATA);
    addRow("MathNode", "MathNode");
    addRow("VariableNode", "VariableNode");
    addRow("TrigonometricNode", "TrigonometricNode");
    addRow("LerpNode", "LerpNode");
    addRow("ClampNode", "ClampNode");
    addRow("GetVariableNode", "GetVariableNode");
    addRow("RandomNode", "RandomNode");

    addCategoryTitle("Transform", NodeCategory::TRANSFORM);
    addRow("RotateNode", "RotateNode");
    addRow("ScaleNode", "ScaleNode");
    addRow("TranslateNode", "TranslateNode");

    addCategoryTitle("Logic", NodeCategory::LOGIC);
    addRow("PrintNode", "PrintNode");
    addRow("CompareNode", "CompareNode");
    addRow("ToggleNode", "ToggleNode");

    addCategoryTitle("Interaction", NodeCategory::INTERACTION);
    addRow("MapperNode", "MapperNode");

    panel->use_fixed_size = false;
    panel->on_children_changed();
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
    
    /*ui::Keyboard::get_instance().initialize();

    graph_container->add_child(ui::Keyboard::get_instance().get_root());*/


    // create graph & editor
    eventGraph = graphManager.createGraph("MainGraph");
    editor = new GraphSystem::GraphEditor(eventGraph,graph_container);

    // red test-box in world
    MeshInstance3D* testBox = new MeshInstance3D();
    testBox->set_name("TestBox");
    testBox->set_position({ 0,1, -10 });
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

    //// instantiate logical nodes
    //auto* runNode = static_cast<GraphSystem::RunNode*>(
    //    editor->createNode("RunNode", "Begin Play", { 550.0f, 550, 0.0f })
    //    );
    //runNode->setEntryPoint(true);

    ////Crear VariableNode para el ángulo
    //auto* angleNode = static_cast<GraphSystem::VariableNode*>(
    //    editor->createNode("VariableNode", "AngleVar", { 400.0f, 200.0f, 0.0f })
    //    );
    //angleNode->setVariableName("angle");  // nombre fijo
    //GraphSystem::VariableNode::setStoredValue("angle", 0.0f); // valor inicial 0.0f

    //// Crear VariableNode para el incremento
    //auto* incrementNode = static_cast<GraphSystem::VariableNode*>(
    //    editor->createNode("VariableNode", "IncrementVar", { 400.0f, 300.0f, 0.0f })
    //    );
    //incrementNode->setVariableName("increment");
    //GraphSystem::VariableNode::setStoredValue("increment", 1.0f); 

    
    // Crear TickNode
    auto* tickNode = static_cast<GraphSystem::TickNode*>(
        editor->createNode("TickNode", "Tick", { 0.0f, -2.0f, 0.0f })
        );

    //tickNode->start();

    //// Crear RotateNode
    //auto* rotateNode = static_cast<GraphSystem::RotateNode*>(
    //    editor->createNode("RotateNode", "Rotator", { 1000.0f, 250.0f, 0.0f })
    //    );

    //// Crear MathNode para sumar ángulo + incremento
    //auto* mathNode = static_cast<GraphSystem::MathNode*>(
    //    editor->createNode("MathNode", "Adder", { 600.0f, 250.0f, 0.0f })
    //    );

    ////Crear VariableNode para el ángulo
    //auto* angleNode = static_cast<GraphSystem::VariableNode*>(
    //    editor->createNode("VariableNode", "AngleVar", { 400.0f, 200.0f, 0.0f })
    //    );
    //angleNode->setVariableName("angle");  // nombre fijo
    //GraphSystem::VariableNode::setStoredValue("angle", 0.0f); // valor inicial 0.0f

    // // Crear VariableNode para el incremento
    //auto* incrementNode = static_cast<GraphSystem::VariableNode*>(
    //    editor->createNode("VariableNode", "IncrementVar", { 400.0f, 300.0f, 0.0f })
    //    );
    //incrementNode->setVariableName("increment");
    //GraphSystem::VariableNode::setStoredValue("increment", 1.0f);



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
            if (!m_is_graph_container_placed) {

                auto* main_camera = get_renderer()->get_camera();
                glm::vec3 eye = main_camera->get_eye();

                glm::vec3 front = -get_renderer()->get_camera_front();

                float distance = 0.5f;

                glm::vec3 target_pos = eye + front * distance;

                glm::mat4 model_transform = glm::mat4(1.0f);
                model_transform = glm::translate(model_transform, target_pos);
                model_transform = model_transform * glm::toMat4(get_rotation_to_face(target_pos, eye, { 0.0f, 1.0f, 0.0f }));
                model_transform = model_transform * glm::rotate(model_transform, glm::radians(180.f), { 1.0f, 0.0f, 0.0f });

                graph_container->set_xr_transform(Transform::mat4_to_transform(model_transform));
                m_is_graph_container_placed = true;
            }

        

            // 2. Grab and Move Logic (using the right controller's grip button)
            if (Input::is_grab_pressed(HAND_RIGHT)) {
               
                    if (!m_is_grabbing_graph_container) {
                        // Start grabbing
                        m_is_grabbing_graph_container = true;

                        // Calculate and store the offset between the controller and the container
                        glm::mat4 controller_transform = Input::get_controller_pose(HAND_RIGHT);
                        glm::mat4 container_transform;

                        container_transform = graph_container->get_global_viewport_model();


                        m_grab_offset_transform = glm::inverse(controller_transform) * container_transform;
                    }

                // While grabbing, update the container's transform based on the controller's movement
                glm::mat4 controller_transform = Input::get_controller_pose(HAND_RIGHT);
                glm::mat4 new_container_transform = controller_transform * m_grab_offset_transform;
                graph_container->set_xr_transform(Transform::mat4_to_transform(new_container_transform));

            }
            else {
                // Stop grabbing when the button is released
                m_is_grabbing_graph_container = false;
            }

            graph_container->update(delta_time); 
        }

        if (run_panel) {

            run_panel->set_xr_transform(Transform::mat4_to_transform(pose));
            run_panel->update(delta_time);
        }
       
    }

    bool select_pressed = Input::was_trigger_pressed(HAND_RIGHT) || Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    if (select_pressed) {

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
                const glm::vec3& intersection_point = ray_direction + ray_direction * distance;

                glm::vec3 spawn = { intersection_point.x + 300.0f,intersection_point.y + 300.0f, 0.f };

                auto* mesh = dynamic_cast<MeshInstance3D*>(node3d);

            
                GraphSystem::GraphNode* node = editor->createNode("EntityNode3D", mesh->get_name(), spawn);

                auto* entityNode = dynamic_cast<GraphSystem::EntityNode3D*>(node);

                entityNode->setEntity(mesh);

                notAllowedNames.push_back(mesh->get_name());

            }
        }

    }

    main_scene->update(delta_time);
}

void SampleEngine::render()
{
    if (show_imgui) {
        render_gui();
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

void SampleEngine::render_gui()
{
    render_default_gui();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open graph (.graph)"))
            {
                std::vector<const char*> filter_patterns = { "*.graph" };
                char const* open_file_name = tinyfd_openFileDialog(
                    "Graph loader",
                    "",
                    filter_patterns.size(),
                    filter_patterns.data(),
                    "Graph format",
                    0
                );

                if (open_file_name) {
                    editor->parse(open_file_name);
                }
            }
            if (ImGui::MenuItem("Save graph (.graph)"))
            {
                std::vector<const char*> filter_patterns = { "*.graph" };

                char const* save_file_name = tinyfd_saveFileDialog(
                    "Graph loader",
                    "",
                    filter_patterns.size(),
                    filter_patterns.data(),
                    "Graph format"
                );

                if (save_file_name) {
                    editor->serialize(save_file_name);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
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
