#include "graph_editor.h"
#include <iostream>

namespace GraphSystem {

    GraphEditor::GraphEditor::GraphEditor(Graph* graph, Scene* scene)
         : graph(graph), scene(scene) {

        GraphSystem::GraphEditor* editor = nullptr;

    }
    GraphNode* GraphEditor::createNode(const std::string& type,
        const std::string& name,
        const glm::vec3& worldPosition) {
        GraphNode* node = nullptr;
        std::string nodeName = name.empty() ? ("node_" + std::to_string(rand())) : name;

        if (type == "PrintNode") {
            node = new PrintNode(nodeName);
        }
        else if (type == "RotateNode") {
            node = new RotateNode(nodeName);
        }
        else if (type == "GraphNode3D") {
            node = new GraphNode3D(nodeName, graph);
        }
        else if (type == "SequenceNode") {
            node = new SequenceNode(nodeName, 2);
        }
        else if (type == "EventNode") {
            node = new EventNode(nodeName);
        }

        if (node) {
            graph->addNode(node);
            std::cout << "[GraphEditor] Created node: " << nodeName << " (" << type << ")\n";

            // Instanciar representación visual si hay escena disponible
            if (scene) {
                auto* widget = new NodeWidget3D(node);
                widget->set_position(worldPosition);
                scene->add_node(widget);
            }
            else {
                std::cerr << "[GraphEditor] Scene is null. Cannot add NodeWidget3D\n";
            }
        }

        return node;
    }


    void GraphEditor::removeNode(GraphNode* node) {
        if (!node) return;
        graph->removeNode(node);
        std::cout << "[GraphEditor] Removed node: " << node->getName() << "\n";
    }

    void GraphEditor::beginConnection(GraphNode* sourceNode, const std::string& outputPort) {
        pendingSource = sourceNode;
        pendingOutput = outputPort;
    }

    void GraphEditor::completeConnection(GraphNode* targetNode, const std::string& inputPort) {
        if (!pendingSource) return;

        if (!graph->connect(pendingSource, pendingOutput, targetNode, inputPort)) {
            std::cerr << "[GraphEditor] Failed to connect " << pendingSource->getName()
                << " to " << targetNode->getName() << "\n";
        }
        else {
            std::cout << "[GraphEditor] Connected " << pendingSource->getName() << " -> "
                << targetNode->getName() << "\n";
        }

        resetConnectionState();
    }

    void GraphEditor::resetConnectionState() {
        pendingSource = nullptr;
        pendingOutput.clear();
    }

}
