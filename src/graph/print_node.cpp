#include "print_node.h"
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>

namespace GraphSystem {

    PrintNode::PrintNode(const std::string& name) : GraphNode(name, NodeCategory::LOGIC) {
        execInput = addInput("Execute", IOType::EXECUTION);
        execOutput = addOutput("Exec", IOType::EXECUTION);

        messageInput = addInput("Message", IOType::FLOAT);  
        std::cout << "[PrintNode] Created: " << name << "\n";

        messageInput->setData(defaultMessage);
    }

    void PrintNode::execute(std::queue<GraphNode*>& executionQueue) {
        
        std::cout << "[PrintNode] TURNO DE EJECUCIÓN PARA: " << getName() << "\n";

        if (messageInput) {
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

        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }

    void PrintNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        uint64_t size = defaultMessage.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(defaultMessage.c_str(), size);
    }

    void PrintNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
    
        uint64_t size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        defaultMessage.resize(size);
        if (size > 0) {
            file.read(&defaultMessage[0], size);
        }
    }

    void PrintNode::rebindPins() {
        
        execInput = getInput("Execute");
        execOutput = getOutput("Exec");
        messageInput = getInput("Message");

       
        if (messageInput) {
            messageInput->setData(defaultMessage);
        }
    }
}
