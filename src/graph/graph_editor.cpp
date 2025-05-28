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
#include "link_renderer_2d.h"
#include "math_node_widget_2d.h"
#include "entity_node_3d.h"
#include "entity_node_widget_2d.h"

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
    else if (type == "GraphNode3D")  node = new GraphNode3D(nodeName, graph);
    else if (type == "SequenceNode") node = new SequenceNode(nodeName, 2);
    else if (type == "RunNode")    node = new RunNode(nodeName);
    else if (type == "MathNode")    node = new MathNode(nodeName);
    else if (type == "BranchNode")    node = new BranchNode(nodeName);
    else if (type == "VariableNode")    node = new VariableNode(nodeName);
    else if (type == "TickNode")    node = new TickNode(nodeName);
    else if (type == "ScaleNode")    node = new ScaleNode(nodeName);
    else if (type == "EntityNode3D") node = new EntityNode3D();



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
    else if (auto tr = dynamic_cast<TranslateNode*>(node)) {
        widget = new TranslateNodeWidget2D(tr, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<ScaleNode*>(node)) {
        widget = new ScaleNodeWidget2D(sc, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<MathNode*>(node)) {
        widget = new MathNodeWidget2D(sc, this, worldPosition);
    }
    else if (auto* en = dynamic_cast<EntityNode3D*>(node))
        widget = new EntityNodeWidget2D(en, this, worldPosition);

    else
        widget = new NodeWidget2D(node, this, worldPosition);


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
