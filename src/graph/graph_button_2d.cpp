#include "graph_button_2d.h"
#include "run_node.h"
#include <iostream>

namespace GraphSystem {

    // El constructor no necesita cambios.
    GraphButton2D::GraphButton2D(const std::string& signal,
        const ui::sButtonDescription& desc,
        Graph* graph,
        GraphEditor* editor,
        const std::string& nodeType)
        : Button2D(signal, desc), graph(graph), editor(editor), node_to_create(nodeType) {
    }

    bool GraphButton2D::on_pressed() {
        std::cout << "[GraphButton2D] Button Pressed!\n";

        // CASO 1: El botón es para crear un nuevo nodo.
        if (editor && !node_to_create.empty()) {
            static int counter = 0;
            std::string nodeName = node_to_create + "_" + std::to_string(counter++);
            editor->createNode(node_to_create, nodeName);
            return true;
        }

        if (!graph) {
            std::cerr << "[GraphButton2D] Este botón no está configurado para ejecutar un grafo (el grafo es nulo).\n";
            return true; 
        }

        GraphNode* startNode = nullptr;
        if (!graph->getEventNodes().empty()) {
            startNode = graph->getEventNodes().front();
        }

        if (startNode) {
            std::cout << "[GraphButton2D] Disparando ejecución desde " << startNode->getName() << "...\n";
            graph->executeFrom(startNode);
        }
        else {
            std::cerr << "[GraphButton2D] No hay un RunNode en el grafo para iniciar la ejecución.\n";
        }

        return true;
    }
}
