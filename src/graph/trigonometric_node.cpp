#include "trigonometric_node.h"
#include <cmath>

namespace GraphSystem {

    TrigonometricNode::TrigonometricNode(const std::string& name, char op)
        : GraphNode(name, NodeCategory::DATA), operation(op), defaultAngle(0.0f)
    {
        angleInput = addInput("Angle", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> float {
            float angle = angleInput->hasData() ? angleInput->getFloat() : defaultAngle;
            float result = 0.0f;

            switch (operation) {
            case 's': result = std::sin(angle); break;
            case 'c': result = std::cos(angle); break;
            case 't':
                result = std::tan(angle);
                if (std::isinf(result) || std::isnan(result)) {
                    result = 0.0f;
                }
                else {
                    result = std::clamp(result, -10.0f, 10.0f);
                }
                break;
            default: result = 0.0f; break;
            }

            return result;
            });

        // Inicializar el valor por defecto
        float initial = std::sin(defaultAngle);  
        resultOutput->setData(initial);
    }

    void TrigonometricNode::setOperation(char op) {
        operation = op;
    }

    void TrigonometricNode::execute()
    {
    }

}
