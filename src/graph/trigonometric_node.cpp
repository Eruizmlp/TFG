#include "trigonometric_node.h"
#include <cmath> 
#include <algorithm> 
#include <limits>    

namespace GraphSystem {

    TrigonometricNode::TrigonometricNode(const std::string& name, char op_char) 
        : GraphNode(name, NodeCategory::DATA), operation(op_char), defaultAngle(0.0f)
    {
        
        angleInput = addInput("Angle", IOType::FLOAT); 
        
        resultOutput = addOutput("Result", IOType::FLOAT); 

        resultOutput->setComputeFunction([this]() -> VariableValue { 
            float angle_rad = angleInput->hasData() ? angleInput->getFloat() : defaultAngle; 
            float result = 0.0f;

            switch (operation) {
            case 's': result = std::sin(angle_rad); break;  
            case 'c': result = std::cos(angle_rad); break; 
            case 't': {

                if (std::cos(angle_rad) == 0.0f) { 
                    result = (std::sin(angle_rad) > 0) ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                }
                else {
                    result = std::tan(angle_rad);
                }

                if (std::isinf(result) || std::isnan(result)) {
                    result = std::copysign(10.0f, result); 
                }
                else {
                    result = std::clamp(result, -10.0f, 10.0f);  
                }
                break;
            }
            default: result = 0.0f; break;  
            }
            return VariableValue(result);
            });

        float initial_result; 
        switch (operation) {
        case 's': initial_result = std::sin(defaultAngle); break;
        case 'c': initial_result = std::cos(defaultAngle); break;
        case 't': initial_result = std::tan(defaultAngle);
            initial_result = std::clamp(initial_result, -10.0f, 10.0f); 
            break;
        default: initial_result = 0.0f; break;
        }
        resultOutput->setData(VariableValue(initial_result)); 
    }

    void TrigonometricNode::setOperation(char op) {
        operation = op;

        float initial_result;
        switch (operation) {
        case 's': initial_result = std::sin(defaultAngle); break;
        case 'c': initial_result = std::cos(defaultAngle); break;
        case 't': initial_result = std::tan(defaultAngle);
            initial_result = std::clamp(initial_result, -10.0f, 10.0f);
            break;
        default: initial_result = 0.0f; break;
        }
        if (resultOutput) resultOutput->setData(VariableValue(initial_result));

    }

    void TrigonometricNode::execute() 
    {
        
    }

}
