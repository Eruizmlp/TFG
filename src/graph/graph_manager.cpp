#include "graph_manager.h"

namespace GraphSystem {

    GraphManager::~GraphManager() {
        for (auto* graph : graphs) {
            delete graph;  // Delete each graph
        }
        graphs.clear();  // Clear list
    }

    Graph* GraphManager::createGraph(const std::string& name) {
        Graph* graph = new Graph(name);
        graphs.push_back(graph);
        return graph;
    }

    const std::vector<Graph*>& GraphManager::getGraphs() const {
        return graphs;
    }

    void GraphManager::removeGraph(Graph* graph) {
        graphs.erase(std::remove(graphs.begin(), graphs.end(), graph), graphs.end());
        delete graph;  // Free memory
    }

    

} // namespace GraphSystem
