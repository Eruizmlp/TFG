#include "graph_editor.h"
#include "node_widget_2d.h"
#include "engine/engine.h"
#include "engine/scene.h"
#include "print_node.h"
#include "sequence_node.h"
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
#include "set_variable_node.h"
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
    else if (type == "SequenceNode") node = new SequenceNode(nodeName, 2);
    else if (type == "RunNode")    node = new RunNode(nodeName);
    else if (type == "MathNode")    node = new MathNode(nodeName);
    else if (type == "BranchNode")    node = new BranchNode(nodeName);
    else if (type == "TickNode")     node = new TickNode(nodeName);
    else if (type == "ScaleNode")    node = new ScaleNode(nodeName);
    else if (type == "EntityNode3D") node = new EntityNode3D();
    else if (type == "TrigonometricNode") node = new TrigonometricNode(nodeName);
    else if (type == "TranslateNode") node = new TranslateNode(nodeName);
    else if (type == "SetVariableNode") node = new SetVariableNode(nodeName, "");
    else if (type == "VariableNode")    node = new VariableNode(nodeName, "");

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



    if (!node) {
        std::cerr << "[GraphEditor] Unknown node type: " << type << "\n";
        return nullptr;
    }


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

    sGraphBinaryHeader header = {
        .node_count = widgets.size(),
        .link_count = graph->getLinks().size(),
    };

    binary_scene_file.write(reinterpret_cast<char*>(&header), sizeof(sGraphBinaryHeader));

    size_t name_size = graph->getName().size();
    binary_scene_file.write(reinterpret_cast<char*>(&name_size), sizeof(size_t));
    binary_scene_file.write(graph->getName().c_str(), name_size);

    // Export all WIDGETS
    for (auto node_widget : widgets) {
        node_widget->serialize(binary_scene_file);
    }

    binary_scene_file.close();
}

void GraphEditor::parse(const std::string& path)
{
    // debug
    widgets.clear();

    std::ifstream binary_scene_file(path, std::ios::in | std::ios::binary);

    if (!binary_scene_file || binary_scene_file.fail()) {
        spdlog::error("GraphEditor: Could not parse file {}", path);
        return;
    }

    sGraphBinaryHeader header;

    binary_scene_file.read(reinterpret_cast<char*>(&header), sizeof(sGraphBinaryHeader));

    // Update name if necessary
    uint64_t name_size = 0;
    binary_scene_file.read(reinterpret_cast<char*>(&name_size), sizeof(uint64_t));
    std::string graph_name;
    graph_name.resize(name_size);
    binary_scene_file.read(&graph_name[0], name_size);
    graph->setName(graph_name);

    std::string node_type;

    for (int i = 0; i < header.node_count; ++i) {

        // parse node type
        uint64_t graph_node_type_size = 0;
        binary_scene_file.read(reinterpret_cast<char*>(&graph_node_type_size), sizeof(uint64_t));
        std::string graph_node_type;
        graph_node_type.resize(graph_node_type_size);
        binary_scene_file.read(&graph_node_type[0], graph_node_type_size);

        // Create corresponding node from the widget data
        GraphNode* graph_node = createNode(graph_node_type);
        // Add to the graph
        graph->addNode(graph_node);

        // Parse widget info (including node data)
        NodeWidget2D* widget = widgets.back();
        widget->parse(binary_scene_file);
    }

    binary_scene_file.close();
}
