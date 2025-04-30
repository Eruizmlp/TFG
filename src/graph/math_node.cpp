#include "math_node.h"

namespace GraphSystem {

    MathNode::MathNode(const std::string& name, char op)
        : GraphNode(name), operation(op)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);
        resultOutput = addOutput("Result", IOType::FLOAT);
    }

    void MathNode::setOperation(char op) {
        operation = op;
    }

    void MathNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float a = aInput->hasData() ? aInput->getFloat() : 0.0f;
        float b = bInput->hasData() ? bInput->getFloat() : 0.0f;
        float res = 0.0f;

        switch (operation) {
        case '+': res = a + b; break;
        case '-': res = a - b; break;
        case '*': res = a * b; break;
        case '/': res = (b != 0.0f ? a / b : 0.0f); break;
        default:  res = 0.0f; break;
        }


        resultOutput->setData(res);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }

} // namespace GraphSystem
