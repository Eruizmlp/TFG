#include "math_node.h"

namespace GraphSystem {

    MathNode::MathNode(const std::string& name, char op)
        : GraphNode(name, NodeCategory::DATA), operation(op)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> float {
            float a = aInput->hasData() ? aInput->getFloat() : aValue;
            float b = bInput->hasData() ? bInput->getFloat() : bValue;

            switch (operation) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return (b != 0.0f) ? a / b : 0.0f;
            default: return 0.0f;  
            }
            });

        resultOutput->setData(aValue + bValue);  
    }

    void MathNode::setOperation(char op) {
        operation = op;
        
    }

}
