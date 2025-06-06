#include "print_node.h"
#include <iostream>
#include <glm/glm.hpp>

namespace GraphSystem {

    PrintNode::PrintNode(const std::string& name) : GraphNode(name, NodeCategory::LOGIC) {
        execInput = addInput("Execute", IOType::EXECUTION);
        execOutput = addOutput("Exec", IOType::EXECUTION);

        messageInput = addInput("Message", IOType::FLOAT);  
        std::cout << "[PrintNode] Created: " << name << "\n";
    }

    void PrintNode::execute(std::queue<GraphNode*>& executionQueue) {
        
        std::cout << "[PrintNode] TURNO DE EJECUCIÓN PARA: " << getName() << "\n";

        if (messageInput && messageInput->hasData()) {
            VariableValue value = messageInput->getValue();

            std::visit([this](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                std::cout << "[PrintNode][" << getName() << "] Valor: ";

                if constexpr (std::is_same_v<T, MeshInstance3D*>) {
                    if (val) std::cout << "MeshInstance3D* (puntero válido)\n";
                    else std::cout << "MeshInstance3D* (nullptr)\n";
                }
                else if constexpr (std::is_same_v<T, glm::vec2>) {
                    std::cout << "vec2(" << val.x << ", " << val.y << ")\n";
                }
                else if constexpr (std::is_same_v<T, glm::vec3>) {
                    std::cout << "vec3(" << val.x << ", " << val.y << ", " << val.z << ")\n";
                }
                else if constexpr (std::is_same_v<T, glm::vec4>) {
                    std::cout << "vec4(" << val.x << ", " << val.y << ", " << val.z << ", " << val.w << ")\n";
                }
                else if constexpr (std::is_same_v<T, glm::mat4>) {
                    std::cout << "mat4(...) (impresión omitida)\n";
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << "\"" << val << "\"\n";
                }
                else {
                    std::cout << val << "\n"; 
                }
                }, value);
        }
        else {
            std::cout << "[PrintNode][" << getName() << "] (No hay datos en la entrada 'Message')\n";
        }

        if (auto* execOutput = getOutput("Exec")) { 
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }
}
