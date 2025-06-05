#include "math_node.h"

namespace GraphSystem {

    MathNode::MathNode(const std::string& name, char op)
        : GraphNode(name, NodeCategory::DATA), operation(op)
    {

        aInput = addInput("A", IOType::FLOAT); 
        bInput = addInput("B", IOType::FLOAT);  
        resultOutput = addOutput("Result", IOType::FLOAT); 

        updateComputeFunction();

        if (mode == MathMode::FLOAT) {
            resultOutput->setData(VariableValue(0.0f)); 
        }
        else { // VEC3
            resultOutput->setData(VariableValue(glm::vec3(0.0f))); 
        }
    }

    void MathNode::setOperation(char op) {
        operation = op; 
        updateComputeFunction();  
    }

    void MathNode::setMode(MathMode newMode) {
        if (mode == newMode) return; 

       
        m_inputs.remove_if([](Input* i) { delete i; return true; });
        m_inputs.clear(); 
        m_outputs.remove_if([](Output* o) { delete o; return true; });
        m_outputs.clear();  


        mode = newMode; 
        if (mode == MathMode::FLOAT) {
            aInput = addInput("A", IOType::FLOAT); 
            bInput = addInput("B", IOType::FLOAT);  
            resultOutput = addOutput("Result", IOType::FLOAT); 
        }
        else { 
            aInput = addInput("A", IOType::VEC3); 
            bInput = addInput("B", IOType::VEC3); 
            resultOutput = addOutput("Result", IOType::VEC3); 
        }

        updateComputeFunction();

        if (mode == MathMode::FLOAT) {
            resultOutput->setData(VariableValue(0.0f)); 
        }
        else { 
            resultOutput->setData(VariableValue(glm::vec3(0.0f))); 
        }
    }

    void MathNode::updateComputeFunction() {
        if (mode == MathMode::FLOAT) {
            resultOutput->setComputeFunction([this]() -> VariableValue { 
                float a = aInput->hasData() ? aInput->getFloat() : aValue;
                float b = bInput->hasData() ? bInput->getFloat() : bValue;
                float result;
                switch (operation) {
                case '+': result = a + b; break;  
                case '-': result = a - b; break; 
                case '*': result = a * b; break;  
                case '/': result = (b != 0.0f) ? a / b : 0.0f; break; 
                default: result = 0.0f; break; 
                }
                return VariableValue(result); 
                });
        }
        else if (mode == MathMode::VEC3) {
            resultOutput->setComputeFunction([this]() -> VariableValue { 
                glm::vec3 a = aInput->hasData() ? aInput->getVec3() : aVec3;
                glm::vec3 b = bInput->hasData() ? bInput->getVec3() : bVec3;
                glm::vec3 result;
                switch (operation) {
                case '+': result = a + b; break; 
                case '-': result = a - b; break; 
                case '*': result = a * b; break;  
                case '/': result = glm::vec3(  
                    (b.x != 0.0f ? a.x / b.x : 0.0f),
                    (b.y != 0.0f ? a.y / b.y : 0.0f),
                    (b.z != 0.0f ? a.z / b.z : 0.0f)
                ); break;
                default: result = glm::vec3(0.0f); break; 
                }
                return VariableValue(result);
                });
        }
    }

}
