#include "get_variable_node.h"
#include "variable_node.h"
#include <fstream>
#include <glm/glm.hpp>

namespace GraphSystem {

  
    GetVariableNode::GetVariableNode(const std::string& name, const std::string& varName, IOType type)
        : GraphNode(name, NodeCategory::DATA), variableName(varName), variableType(type)
    {
  
        outValue = addOutput("Value", variableType);

        switch (variableType) {
        case IOType::BOOL:   defaultValue = false; break;
        case IOType::INT:    defaultValue = 0; break;
        case IOType::FLOAT:  defaultValue = 0.0f; break;
        case IOType::STRING: defaultValue = std::string(""); break;
        case IOType::VEC2:   defaultValue = glm::vec2(0.0f); break;
        case IOType::VEC3:   defaultValue = glm::vec3(0.0f); break;
        case IOType::VEC4:   defaultValue = glm::vec4(0.0f); break;
        case IOType::MAT4:   defaultValue = glm::mat4(1.0f); break;
        case IOType::MESH:   defaultValue = static_cast<MeshInstance3D*>(nullptr); break;
        default:             defaultValue = 0.0f; 
        }

        outValue->setComputeFunction([varName = this->variableName, defVal = this->defaultValue]() -> VariableValue {
            return VariableNode::getStoredValue(varName, defVal);
            });

        outValue->setData(VariableNode::getStoredValue(variableName, defaultValue));
    }

    void GetVariableNode::execute(std::queue<GraphNode*>& executionQueue) {}


    void GetVariableNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        uint64_t name_size = variableName.size();
        file.write(reinterpret_cast<const char*>(&name_size), sizeof(name_size));
        file.write(variableName.c_str(), name_size);

        file.write(reinterpret_cast<const char*>(&variableType), sizeof(variableType));

        serializeVariableValue(file, defaultValue);
    }

    void GetVariableNode::parse(std::ifstream& file) {
        GraphNode::parse(file);

        uint64_t name_size = 0;
        file.read(reinterpret_cast<char*>(&name_size), sizeof(name_size));
        variableName.resize(name_size);
        if (name_size > 0) {
            file.read(&variableName[0], name_size);
        }


        file.read(reinterpret_cast<char*>(&variableType), sizeof(variableType));

       
        defaultValue = parseVariableValue(file);
    }

    void GetVariableNode::rebindPins() {
        outValue = getOutput("Value");

        if (outValue) {
            
            outValue->setComputeFunction([varName = this->variableName, defVal = this->defaultValue]() -> VariableValue {
                return VariableNode::getStoredValue(varName, defVal);
                });

           
            outValue->setData(VariableNode::getStoredValue(variableName, defaultValue));
        }
    }
}
