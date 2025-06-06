#include "trigonometric_node.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <queue>

namespace GraphSystem {

    TrigonometricNode::TrigonometricNode(const std::string& name, char op_char)
        : GraphNode(name, NodeCategory::DATA), operation(op_char), defaultAngle(0.0f)
    {
        angleInput = addInput("Angle", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> VariableValue {
            float angle_rad = defaultAngle;
            if (angleInput && angleInput->hasData()) {
                VariableValue v = angleInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) {
                    angle_rad = *pval;
                }
                else {
                    std::cerr << "[TrigonometricNode] (" << getName() << ") Type mismatch for Angle input. Expected FLOAT.\n";
                }
            }

            float result = 0.0f;
            switch (operation) {
            case 's': result = std::sin(angle_rad); break;
            case 'c': result = std::cos(angle_rad); break;
            case 't': {

                if (std::abs(std::cos(angle_rad)) < 1e-6) {
                    result = std::numeric_limits<float>::max(); 
                }
                else {
                    result = std::tan(angle_rad);
                }

                result = std::clamp(result, -10000.0f, 10000.0f);
                break;
            }
            default: result = 0.0f; break;
            }
            return VariableValue(result);
            });

        resultOutput->setData(VariableValue(std::sin(defaultAngle)));
    }

    void TrigonometricNode::setOperation(char op) {
        operation = op;
    }

    void TrigonometricNode::execute(std::queue<GraphNode*>& executionQueue) {
        
    }
}
