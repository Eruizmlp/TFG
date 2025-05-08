#include "graph_manager.h"

namespace GraphSystem {

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
        delete graph;  
    }

    
    void GraphManager::executeGraph(Graph* graph) {
        if (graph) {
            graph->triggerEntryPoints();
        }
    }

    void GraphManager::renderGraphs() {
        for (const auto& graph : graphs) {
            graph->render();
        }
    }

    GraphManager::~GraphManager() {
        for (auto graph : graphs) {
            delete graph; 
        }
        graphs.clear();  
    }

} 
