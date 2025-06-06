#include "run_node.h"
#include <iostream>
#include <queue>

namespace GraphSystem {

    RunNode::RunNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW),
        eventName("DefaultEvent"),
        execOutput(addOutput("Execution", IOType::EXECUTION))
    {
        setEntryPoint(true);
    }

    RunNode::~RunNode() {
        std::cout << "[RunNode] Destruyendo evento: " << eventName << "\n";
    }

    
    void RunNode::execute(std::queue<GraphNode*>& executionQueue) {

        // 1. Lógica del nodo
        std::cout << "[RunNode] Evento '" << eventName << "' disparado.\n";

        // 2. Propagación
        if (execOutput) {
            std::cout << "[RunNode] Buscando nodos conectados para propagar...\n"; 
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    // --- MENSAJE DE DEPURACIÓN CLAVE ---
                    std::cout << "[RunNode] AÑADIENDO A LA COLA: " << nextNode->getName() << "\n";
                    executionQueue.push(nextNode);
                }
            }
        }
        else {
            std::cout << "[RunNode] El pin de salida 'Execution' es nulo.\n"; 
        }
    }
}
