#pragma once

#include "graph.h"
#include "graph_node.h"
#include <string>
#include <vector>

namespace GraphSystem { class NodeWidget2D; }

namespace GraphSystem {

    class GraphEditor {
    public:
        explicit GraphEditor(Graph* graph);

        // Creates a logical node + its 2D widget in the main scene
        GraphNode* createNode(const std::string& type,
            const std::string& name,
            const glm::vec3& worldPosition = glm::vec3(800));

        // Connection workflow
        void beginConnection(GraphNode* sourceNode, const std::string& outputPort);
        void completeConnection(GraphNode* targetNode, const std::string& inputPort);
        void resetConnectionState();

        // Expose widgets for selection
        const std::vector<NodeWidget2D*>& getWidgets() const { return widgets; }

    private:
        Graph* graph;
        std::vector<NodeWidget2D*> widgets;
        GraphNode* pendingSource = nullptr;
        std::string pendingOutput;
    };

}
