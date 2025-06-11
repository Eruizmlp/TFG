#include "graph_button_2d.h"
#include "run_node.h"

namespace GraphSystem {

    GraphButton2D::GraphButton2D(
        const std::string& signal,
        const ui::sButtonDescription& desc,
        Graph* graph,
        GraphEditor* editor,
        const std::string& nodeType)
        : TextureButton2D(signal, desc), graph(graph), editor(editor), node_to_create(nodeType)
    {
    }

    bool GraphButton2D::on_pressed() {

        if (editor && !node_to_create.empty()) {
            static int counter = 0;
            std::string nodeName = node_to_create + "_" + std::to_string(counter++);
            editor->createNode(node_to_create, nodeName);
            return true;
        }

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

        return true;
    }
}
