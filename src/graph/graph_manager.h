#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <vector>
#include "graph.h"

namespace GraphSystem {

    class GraphManager {
    private:
        std::vector<Graph*> graphs;

    public:
        GraphManager() = default;
        ~GraphManager();

        // Create and manage new graph
        Graph* createGraph(const std::string& name);

        // Access to managed graphs
        const std::vector<Graph*>& getGraphs() const;

        // Remove and delete a specific graph
        void removeGraph(Graph* graph);

    };

} // namespace GraphSystem

#endif // GRAPH_MANAGER_H
