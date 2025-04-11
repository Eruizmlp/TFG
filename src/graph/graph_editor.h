#pragma once
#include "graph.h"
#include "graph_node.h"
#include "graph_node3D.h"
#include "event_node.h"
#include "sequence_node.h"
#include "print_node.h"
#include "rotate_node.h"
#include "node_widget_3d.h"     
#include "engine/scene.h"       

#include <string>
#include <vector>

namespace GraphSystem {

    class GraphEditor {
    public:
        GraphEditor(Graph* graph, Scene* scene);

        // Node management
        GraphNode* createNode(const std::string& type,
            const std::string& name,
            const glm::vec3& worldPosition = glm::vec3(0));

        void removeNode(GraphNode* node);

        // Connection management
        void beginConnection(GraphNode* sourceNode, const std::string& outputPort);
        void completeConnection(GraphNode* targetNode, const std::string& inputPort);

        // Utility
        void resetConnectionState();

    private:
        Graph* graph;
        Scene* scene; 

        GraphNode* pendingSource = nullptr;
        std::string pendingOutput;
    };
}
