#include "trigonometric_node.h"
#include <cmath>

namespace GraphSystem {

    TrigonometricNode::TrigonometricNode(const std::string& name, char op)
        : GraphNode(name, NodeCategory::DATA), operation(op), defaultAngle(0.0f)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        angleInput = addInput("Angle", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        resultOutput = addOutput("Result", IOType::FLOAT);
    }

    void TrigonometricNode::setOperation(char op) {
        operation = op;
    }

    void TrigonometricNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float angle = angleInput->hasData() ? angleInput->getFloat() : defaultAngle;
        float result = 0.0f;

        switch (operation) {
        case 's': result = std::sin(angle); break;
        case 'c': result = std::cos(angle); break;
        case 't': result = std::tan(angle); break;
        default: result = 0.0f; break;
        }

        resultOutput->setData(result);

        for (auto& link : execOutput->getLinks()) {
            if (auto* next = link->getTargetNode()) {
                next->setExecutionPending(true);
            }
        }
    }

}
