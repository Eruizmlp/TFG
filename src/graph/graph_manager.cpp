#include "graph_manager.h"

namespace GraphSystem {

    Graph* GraphManager::createGraph(const std::string& name, bool isTickGraph) {
        Graph* graph = new Graph(name);
        graphs.push_back(graph);
        return graph;
    }

    const std::vector<Graph*>& GraphManager::getGraphs() const {
        return graphs;
    }

    void GraphManager::removeGraph(Graph* graph) {
        graphs.erase(std::remove(graphs.begin(), graphs.end(), graph), graphs.end());
        delete graph;  // Clean up the graph object
    }

    
    void GraphManager::executeGraph(Graph* graph) {
        if (graph) {
            graph->execute();
        }
    }

    void GraphManager::renderGraphs() {
        for (const auto& graph : graphs) {
            graph->render();
        }
    }

    // Destructor to clean up all dynamically allocated graphs
    GraphManager::~GraphManager() {
        for (auto graph : graphs) {
            delete graph;  // Delete each graph object
        }
        graphs.clear();  // Clear the list
    }

} // namespace GraphSystem
