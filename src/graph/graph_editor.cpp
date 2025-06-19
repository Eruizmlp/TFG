#include "graph_editor.h"
#include "node_widget_2d.h"
#include "engine/engine.h"
#include "engine/scene.h"
#include "print_node.h"
//#include "sequence_node.h"
#include "clamp_node.h"
#include "compare_node.h"
#include "get_variable_node.h"
#include "lerp_node.h"
#include "loop_node.h"
#include "mapper_node.h"
#include "random_node.h"
#include "timer_node.h"
#include "toggle_node.h"
#include "mapper_node_widget_2d.h"
#include "compare_node_widget_2d.h"
#include "run_node.h"
#include "math_node.h"
#include "branch_node.h"
#include "variable_node.h"
#include "tick_node.h"
#include "tick_node_widget_2d.h"
#include <iostream>
#include "link_renderer_2d.h"
#include "math_node_widget_2d.h"
#include "entity_node_3d.h"
#include "entity_node_widget_2d.h"
#include "trigonometric_node.h"
#include "trigonometric_node_widget_2d.h"
//#include "set_variable_node.h"
#include "keyboard.h"
#include <fstream>
#include "spdlog/spdlog.h"

using namespace GraphSystem;

GraphEditor::GraphEditor(Graph* graph, Node2D* panel)
    : graph(graph), graph_container(panel)
{
    visualLink = new LinkRenderer2D(graph, this);
    if (graph_container) {
        graph_container->add_child(visualLink);
    }
    else if (auto* eng = Engine::get_instance(); eng && eng->get_main_scene()) {
        eng->get_main_scene()->add_node(visualLink);
    }
}

GraphNode* GraphEditor::createNode(const std::string& type,
    const std::string& name,
    const glm::vec3& worldPosition)
{
    GraphNode* node = nullptr;
    std::string nodeName = name.empty()
        ? ("node_" + std::to_string(rand()))
        : name;

    if (type == "PrintNode")         node = new PrintNode(nodeName);
    else if (type == "RotateNode")   node = new RotateNode(nodeName);
   // else if (type == "SequenceNode") node = new SequenceNode(nodeName, 2);
    else if (type == "RunNode")    node = new RunNode(nodeName);
    else if (type == "MathNode")    node = new MathNode(nodeName);
    else if (type == "BranchNode")    node = new BranchNode(nodeName);
    else if (type == "TickNode")     node = new TickNode(nodeName);
    else if (type == "ScaleNode")    node = new ScaleNode(nodeName);
    else if (type == "EntityNode3D") node = new EntityNode3D(nodeName);
    else if (type == "TrigonometricNode") node = new TrigonometricNode(nodeName);
    else if (type == "TranslateNode") node = new TranslateNode(nodeName);
   // else if (type == "SetVariableNode") node = new SetVariableNode(nodeName, "");
    else if (type == "VariableNode")    node = new VariableNode(nodeName, "");
    else if (type == "ClampNode")       node = new ClampNode(nodeName);
    else if (type == "CompareNode")     node = new CompareNode(nodeName);
    else if (type == "GetVariableNode") node = new GetVariableNode(nodeName, ""); // Assumes default empty var name
    else if (type == "LerpNode")        node = new LerpNode(nodeName);
    else if (type == "LoopNode")        node = new LoopNode(nodeName);
    else if (type == "MapperNode")      node = new MapperNode(nodeName);
    else if (type == "RandomNode")      node = new RandomNode(nodeName);
    else if (type == "TimerNode")       node = new TimerNode(nodeName);
    else if (type == "ToggleNode")      node = new ToggleNode(nodeName);

    //else if (type == "VariableNode") {
    //    auto* varNode = new VariableNode(nodeName);

    //    ui::Keyboard::get_instance().request(
    //        [varNode](const std::string& userInput) {
    //            varNode->setVariableName(userInput);
    //        },
    //        "defaultVar",
    //        32
    //    );

    //    node = varNode;
    //}

    graph->addNode(node);

    if (!node) {
        std::cerr << "[GraphEditor] Unknown node type: " << type << "\n";
        return nullptr;
    }

    glm::vec3 localPosition = worldPosition;

    glm::vec2 containerPos = graph_container->get_translation();
    localPosition.x -= containerPos.x;
    localPosition.y -= containerPos.y;
        


    // choose the right widget subclass
    NodeWidget2D* widget = nullptr;
    if (auto* rn = dynamic_cast<RotateNode*>(node))
        widget = new RotateNodeWidget2D(type, rn, this, worldPosition);
    else if (auto tr = dynamic_cast<TranslateNode*>(node)) {
        widget = new TranslateNodeWidget2D(type, tr, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<ScaleNode*>(node)) {
        widget = new ScaleNodeWidget2D(type, sc, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<MathNode*>(node)) {
        widget = new MathNodeWidget2D(type, sc, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<TrigonometricNode*>(node)) {
        widget = new TrigonometricNodeWidget2D(type, sc, this, worldPosition);
    }
    else if (auto* tk = dynamic_cast<TickNode*>(node))
        widget = new TickNodeWidget2D(type, tk, this, worldPosition);

    else if (auto* cn = dynamic_cast<CompareNode*>(node)) {
        widget = new CompareNodeWidget2D(type, cn, this, worldPosition);
    }
    else if (auto* mn = dynamic_cast<MapperNode*>(node)) {
        widget = new MapperNodeWidget2D(type, mn, this, worldPosition);
    }

    else
        widget = new NodeWidget2D(type, node, this, worldPosition);


    if (graph_container) {
        graph_container->add_child(widget);
        
    }
    else if (auto* eng = Engine::get_instance(); eng && eng->get_main_scene()) {
        eng->get_main_scene()->add_node(widget);
    }
    else {
        std::cout << "[GraphEditor] graph_container is null!\n";
    }
    
    
    widgets.push_back(widget);
    return node;
}

void GraphEditor::beginConnection(GraphNode* sourceNode,
    const std::string& outputPort)
{
    pendingSource = sourceNode;
    pendingOutput = outputPort;
}

void GraphEditor::completeConnection(GraphNode* targetNode,
    const std::string& inputPort)
{
    if (!pendingSource) return;

    if (!graph->connect(pendingSource, pendingOutput, targetNode, inputPort)) {
        std::cerr << "[GraphEditor] Failed to connect "
            << pendingSource->getName() << " -> "
            << targetNode->getName() << "\n";
    }
    else {
        std::cout << "[GraphEditor] Connected "
            << pendingSource->getName() << " -> "
            << targetNode->getName() << "\n";
    }

    resetConnectionState();
}

void GraphEditor::resetConnectionState()
{
    pendingSource = nullptr;
    pendingOutput.clear();
}

void GraphEditor::update(float delta_time)
{

    if (visualLink) visualLink->update(delta_time);

}

void GraphEditor::serialize(const std::string& path)
{
    std::ofstream binary_scene_file(path, std::ios::out | std::ios::binary);
    if (!binary_scene_file) {
        spdlog::error("GraphEditor: Could not open file for writing {}", path);
        return;
    }

    // --- Header ---
    sGraphBinaryHeader header = {
        .node_count = widgets.size(),
        .link_count = graph->getLinks().size(),
    };
    binary_scene_file.write(reinterpret_cast<char*>(&header), sizeof(sGraphBinaryHeader));

    // --- Graph Name ---
    uint64_t graph_name_size = graph->getName().size();
    binary_scene_file.write(reinterpret_cast<char*>(&graph_name_size), sizeof(uint64_t));
    binary_scene_file.write(graph->getName().c_str(), graph_name_size);

    // --- 1. Serialize All Widgets/Nodes ---
    for (auto* node_widget : widgets) {
        node_widget->serialize(binary_scene_file);
    }

    // --- 2. Serialize All Links ---
    for (const auto* link : graph->getLinks()) {
        // Source Node Name
        std::string source_node_name = link->getSourceNode()->getName();
        uint64_t source_name_size = source_node_name.size(); 
        binary_scene_file.write(reinterpret_cast<const char*>(&source_name_size), sizeof(uint64_t)); // <--- CHANGE HERE
        binary_scene_file.write(source_node_name.c_str(), source_name_size);

        // Output Port Name
        std::string output_port_name = link->getOutput()->getName();
        uint64_t output_port_size = output_port_name.size(); 
        binary_scene_file.write(reinterpret_cast<const char*>(&output_port_size), sizeof(uint64_t)); // <--- CHANGE HERE
        binary_scene_file.write(output_port_name.c_str(), output_port_size);

        // Target Node Name
        std::string target_node_name = link->getTargetNode()->getName();
        uint64_t target_name_size = target_node_name.size(); 
        binary_scene_file.write(reinterpret_cast<const char*>(&target_name_size), sizeof(uint64_t)); // <--- CHANGE HERE
        binary_scene_file.write(target_node_name.c_str(), target_name_size);

        // Input Port Name
        std::string input_port_name = link->getTargetInput()->getName();
        uint64_t input_port_size = input_port_name.size(); 
        binary_scene_file.write(reinterpret_cast<const char*>(&input_port_size), sizeof(uint64_t)); 
        binary_scene_file.write(input_port_name.c_str(), input_port_size);
    }

    binary_scene_file.close();
}

void GraphEditor::parse(const std::string& path)
{
    std::ifstream binary_scene_file(path, std::ios::in | std::ios::binary);
    if (!binary_scene_file) {
        spdlog::error("GraphEditor: Could not parse file {}", path);
        return;
    }

    graph->clear();
    for (auto* w : widgets) { delete w; }
    widgets.clear();

    sGraphBinaryHeader header;
    binary_scene_file.read(reinterpret_cast<char*>(&header), sizeof(sGraphBinaryHeader));

    uint64_t graph_name_size = 0;
    binary_scene_file.read(reinterpret_cast<char*>(&graph_name_size), sizeof(uint64_t));
    std::string graph_name;
    graph_name.resize(graph_name_size);
    binary_scene_file.read(&graph_name[0], graph_name_size);
    graph->setName(graph_name);

    // Pass 1: Create, Parse, and Rebind all nodes
    for (uint64_t i = 0; i < header.node_count; ++i) {
        uint64_t node_type_size = 0;
        binary_scene_file.read(reinterpret_cast<char*>(&node_type_size), sizeof(uint64_t));
        std::string graph_node_type;
        graph_node_type.resize(node_type_size);
        binary_scene_file.read(&graph_node_type[0], node_type_size);

        GraphNode* new_node = createNode(graph_node_type, "TEMP");
        if (!new_node) continue;
        NodeWidget2D* new_widget = widgets.back();

        new_widget->parse(binary_scene_file);

        new_node->rebindPins();
    }

    // --- Pass 2: Connect all links ---
    for (uint64_t i = 0; i < header.link_count; ++i) {
        // THIS IS THE CRITICAL FIX FOR THE FREEZE
        uint64_t source_name_size, output_port_size, target_name_size, input_port_size;

        binary_scene_file.read(reinterpret_cast<char*>(&source_name_size), sizeof(uint64_t));
        std::string source_node_name(source_name_size, '\0');
        binary_scene_file.read(&source_node_name[0], source_name_size);

        binary_scene_file.read(reinterpret_cast<char*>(&output_port_size), sizeof(uint64_t));
        std::string output_port_name(output_port_size, '\0');
        binary_scene_file.read(&output_port_name[0], output_port_size);

        binary_scene_file.read(reinterpret_cast<char*>(&target_name_size), sizeof(uint64_t));
        std::string target_node_name(target_name_size, '\0');
        binary_scene_file.read(&target_node_name[0], target_name_size);

        binary_scene_file.read(reinterpret_cast<char*>(&input_port_size), sizeof(uint64_t));
        std::string input_port_name(input_port_size, '\0');
        binary_scene_file.read(&input_port_name[0], input_port_size);

        GraphNode* source_node = graph->getNodeByName(source_node_name);
        GraphNode* target_node = graph->getNodeByName(target_node_name);

        if (source_node && target_node) {
            graph->connect(source_node, output_port_name, target_node, input_port_name);
        }
        else {
            spdlog::error("Could not find nodes for link: {} -> {}", source_node_name, target_node_name);
        }
    }

    // --- Pass 3: Post-Processing for Scene Entities ---
    Scene* scene = Engine::get_instance()->get_main_scene();
    if (scene) {
        const auto& all_scene_nodes = scene->get_nodes();
        for (auto* widget : widgets) {
            if (auto* entityNode = dynamic_cast<EntityNode3D*>(widget->getLogicNode())) {
                const std::string& entity_name_to_find = entityNode->getEntityNameOnLoad();
                if (!entity_name_to_find.empty()) {
                    for (Node* current_scene_node : all_scene_nodes) {
                        if (current_scene_node->get_name() == entity_name_to_find) {
                            if (auto* meshInstance = dynamic_cast<MeshInstance3D*>(current_scene_node)) {
                                entityNode->setEntity(meshInstance);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    binary_scene_file.close();
}
