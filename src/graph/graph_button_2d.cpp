#include "graph_button_2d.h"
#include "run_node.h" 

namespace GraphSystem {

    GraphButton2D::GraphButton2D(
        const std::string& signal,
        const ui::sButtonDescription& desc,
        Graph* graph)
        : TextureButton2D(signal, desc), graph(graph)
    {
    }

    bool GraphButton2D::on_pressed() {

        if (graph) {
            GraphNode* startNode = nullptr;
            
            if (!graph->getEventNodes().empty()) {
                startNode = graph->getEventNodes().front();
            }
    
            if (startNode) {
                graph->executeFrom(startNode);
            }
            return true;
        }
        return false;
    }
}
