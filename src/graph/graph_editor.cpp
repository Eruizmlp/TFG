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
#include "graph_node3D.h"
#include "tick_node.h"
#include <iostream>
#include "visual_link_2d.h"


using namespace GraphSystem;

GraphEditor::GraphEditor(Graph* graph, Node2D* panel)
    : graph(graph), graph_container(panel)
{
    visualLink = new VisualLink2D(graph, this);
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
    else if (type == "GraphNode3D")  node = new GraphNode3D(nodeName, graph);
    else if (type == "SequenceNode") node = new SequenceNode(nodeName, 2);
    else if (type == "RunNode")    node = new RunNode(nodeName);
    else if (type == "MathNode")    node = new MathNode(nodeName);
    else if (type == "BranchNode")    node = new BranchNode(nodeName);
    else if (type == "VariableNode")    node = new VariableNode(nodeName);
    else if (type == "TickNode")    node = new TickNode(nodeName);
    else if (type == "ScaleNode")    node = new ScaleNode(nodeName);

    if (!node) {
        std::cerr << "[GraphEditor] Unknown node type: " << type << "\n";
        return nullptr;
    }

    graph->addNode(node);
    std::cout << "[GraphEditor] Created node: " << nodeName
        << " (" << type << ")\n";

    
    // choose the right widget subclass
    NodeWidget2D* widget = nullptr;
    if (auto* rn = dynamic_cast<RotateNode*>(node))
        widget = new RotateNodeWidget2D(rn, this, worldPosition);
    else
        widget = new NodeWidget2D(node, this, worldPosition);


    if (graph_container) {
        graph_container->add_child(widget);
    }
    else if (auto* eng = Engine::get_instance(); eng && eng->get_main_scene()) {
        eng->get_main_scene()->add_node(widget);
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
