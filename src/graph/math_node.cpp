#include "math_node.h"
#include <glm/glm.hpp>
#include <variant>
#include <iostream>


#include <sstream>
#include <fstream>

namespace GraphSystem {

    MathNode::MathNode(const std::string& name, char op)
        : GraphNode(name, NodeCategory::DATA), operation(op)
    {
        
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);
        
        resultOutput->setComputeFunction([this]() -> VariableValue {
            auto aVal = aInput->getValue();
            auto bVal = bInput->getValue();

            if (std::holds_alternative<float>(aVal) && std::holds_alternative<float>(bVal)) {
                float a = std::get<float>(aVal);
                float b = std::get<float>(bVal);
                switch (operation) {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return (b != 0.0f) ? a / b : 0.0f;
                default: return 0.0f;
                }
            }
            else if (std::holds_alternative<glm::vec3>(aVal) && std::holds_alternative<glm::vec3>(bVal)) {
                glm::vec3 a = std::get<glm::vec3>(aVal);
                glm::vec3 b = std::get<glm::vec3>(bVal);
                switch (operation) {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return a / glm::max(b, glm::vec3(0.0001f));
                default: return glm::vec3(0.0f);
                }
            }
            else {
                std::cerr << "[MathNode] (" << getName() << ") Type mismatch or unsupported types.\n";
                return 0.0f; 
            }
            });

        resultOutput->setData(0.0f);
    }

    void MathNode::execute(std::queue<GraphNode*>& executionQueue)
    {
    }


    void MathNode::setOperation(char op) {
        operation = op;
    }
    void MathNode::serialize(std::ofstream& file)
    {
        GraphNode::serialize(file);
        file.write(reinterpret_cast<const char*>(&operation), sizeof(operation));

    }
    void MathNode::parse(std::ifstream& file)
    {
        GraphNode::parse(file);
        file.read(reinterpret_cast<char*>(&operation), sizeof(operation));
    }

    void MathNode::rebindPins()
    {
        aInput = getInput("A");
        bInput = getInput("B");
        resultOutput = getOutput("Result");

    }
}
