#include "trigonometric_node.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <queue>
#include <fstream>

namespace GraphSystem {

    TrigonometricNode::TrigonometricNode(const std::string& name, char op_char)
        : GraphNode(name, NodeCategory::DATA), operation(op_char), defaultAngle(0.0f)
    {
        angleInput = addInput("Angle", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

       
        angleInput->setData(defaultAngle);

        resultOutput->setComputeFunction([this]() -> VariableValue {
            float angle_rad = angleInput->getFloat();
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
            return result;
            });

        resultOutput->setData(std::sin(defaultAngle));
    }

    void TrigonometricNode::setOperation(char op) {
        operation = op;
    }

    void TrigonometricNode::execute(std::queue<GraphNode*>& executionQueue) {}



    void TrigonometricNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        file.write(reinterpret_cast<const char*>(&operation), sizeof(operation));
        file.write(reinterpret_cast<const char*>(&defaultAngle), sizeof(defaultAngle));
    }

    void TrigonometricNode::parse(std::ifstream& file) {
        GraphNode::parse(file);

        file.read(reinterpret_cast<char*>(&operation), sizeof(operation));
        file.read(reinterpret_cast<char*>(&defaultAngle), sizeof(defaultAngle));
    }

    void TrigonometricNode::rebindPins() {

        angleInput = getInput("Angle");
        resultOutput = getOutput("Result");

        if (angleInput) {
            angleInput->setData(defaultAngle);
        }

        if (resultOutput) {
            resultOutput->setComputeFunction([this]() -> VariableValue {
                if (!angleInput) return 0.0f; 
                float angle_rad = angleInput->getFloat();
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
                return result;
                });
          
            resultOutput->setData(resultOutput->getComputeFunction()());
        }
    }
}
