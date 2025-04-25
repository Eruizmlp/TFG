#include "graph_editor.h"
#include "node_widget_2d.h"
#include "engine/engine.h"
#include "engine/scene.h"
#include "print_node.h"
#include "sequence_node.h"
#include "event_node.h"
#include "rotate_node.h"
#include "graph_node3D.h"  
#include <iostream>

using namespace GraphSystem;

GraphEditor::GraphEditor(Graph* graph)
    : graph(graph)
{
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
    else if (type == "GraphNode3D")  node = new GraphNode3D(nodeName, graph);
    else if (type == "SequenceNode") node = new SequenceNode(nodeName, 2);
    else if (type == "EventNode")    node = new EventNode(nodeName);

    if (!node) {
        std::cerr << "[GraphEditor] Unknown node type: " << type << "\n";
        return nullptr;
    }

    graph->addNode(node);
    std::cout << "[GraphEditor] Created node: " << nodeName
        << " (" << type << ")\n";

    
    auto* eng = Engine::get_instance();
    if (eng && eng->get_main_scene()) {
        auto* widget = new NodeWidget2D(node, this, worldPosition);
        eng->get_main_scene()->add_node(widget);
        widgets.push_back(widget);
    }
    else {
        std::cerr << "[GraphEditor] No main scene available\n";
    }

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
