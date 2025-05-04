#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <vector>
#include "graph.h"

namespace GraphSystem {

    class GraphManager {
    private:
        std::vector<Graph*> graphs;

    public:
        // Create a new graph and return a raw pointer to it
        Graph* createGraph(const std::string& name, bool isTickGraph = false);

        // Get the list of graphs
        const std::vector<Graph*>& getGraphs() const;

        // Remove a graph from the manager
        void removeGraph(Graph* graph);

        // Execute a specific graph
        void executeGraph(Graph* graph);

        // Render all graphs
        void renderGraphs();

        ~GraphManager();
    };

} 

#endif // GRAPH_MANAGER_H
