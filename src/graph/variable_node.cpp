
#include "variable_node.h"
#include <iostream>
#include <queue>
#include <optional>

#include <sstream>
#include <fstream>

namespace GraphSystem {

    std::unordered_map<std::string, VariableValue> VariableNode::variableStore;

    VariableNode::VariableNode(const std::string& name, const std::string& varName, const VariableValue& initialValue)
        : GraphNode(name, NodeCategory::DATA), variableName(varName), defaultValue(initialValue)
    {
        IOType ioType = IOType::FLOAT;
        if (std::holds_alternative<float>(initialValue)) ioType = IOType::FLOAT;
        else if (std::holds_alternative<int>(initialValue)) ioType = IOType::INT;
        else if (std::holds_alternative<bool>(initialValue)) ioType = IOType::BOOL;
        else if (std::holds_alternative<std::string>(initialValue)) ioType = IOType::STRING;
        else if (std::holds_alternative<glm::vec2>(initialValue)) ioType = IOType::VEC2;
        else if (std::holds_alternative<glm::vec3>(initialValue)) ioType = IOType::VEC3;
        else if (std::holds_alternative<glm::vec4>(initialValue)) ioType = IOType::VEC4;
        else if (std::holds_alternative<glm::mat4>(initialValue)) ioType = IOType::MAT4;
        else if (std::holds_alternative<MeshInstance3D*>(initialValue)) ioType = IOType::MESH;

        setupIO(ioType);

        if (variableStore.find(variableName) == variableStore.end() && !variableName.empty()) {
            variableStore[variableName] = defaultValue;
        }


        outValue->setComputeFunction([varName = this->variableName, defVal = this->defaultValue]() -> VariableValue {
            return VariableNode::getStoredValue(varName, defVal);
            });
    }

    void VariableNode::setupIO(IOType type) {
        execInput = addInput("Execute", IOType::EXECUTION);
        valueInput = addInput("Value", type);
        execOutput = addOutput("Exec", IOType::EXECUTION);
        outValue = addOutput("Value", type);
    }

   void VariableNode::setVariableName(const std::string& varName) {
        variableName = varName;
        
        if (!variableName.empty()) {
            variableStore.try_emplace(variableName, defaultValue);
        }

        if (outValue) {
            outValue->setComputeFunction([varName = this->variableName, defVal = this->defaultValue]() -> VariableValue {
                return VariableNode::getStoredValue(varName, defVal);
            });
        }
    }


    const std::string& VariableNode::getVariableName() const {
        return variableName;
    }

    void VariableNode::execute(std::queue<GraphNode*>& executionQueue) {

        if (valueInput && valueInput->hasData()) {
            variableStore[variableName] = valueInput->getValue();
        }

        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }

    VariableValue VariableNode::getStoredValue(const std::string& varName, const VariableValue& defaultValue) {
        if (varName.empty()) return defaultValue;
        auto it = variableStore.find(varName);
        return (it != variableStore.end()) ? it->second : defaultValue;
    }

    void VariableNode::setStoredValue(const std::string& varName, const VariableValue& value) {
        variableStore[varName] = value;
    }

    void VariableNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
        uint64_t var_name_size = variableName.size();
        file.write(reinterpret_cast<const char*>(&var_name_size), sizeof(var_name_size));
        file.write(variableName.c_str(), var_name_size);

        serializeVariableValue(file, defaultValue);


    }

    void VariableNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
        uint64_t var_name_size = 0;
        file.read(reinterpret_cast<char*>(&var_name_size), sizeof(var_name_size));
        variableName.resize(var_name_size);
        if (var_name_size > 0) {
            file.read(&variableName[0], var_name_size);
        }

        defaultValue = parseVariableValue(file);
    }

    void VariableNode::rebindPins() {


        execInput = getInput("Execute");
        valueInput = getInput("Value");
        execOutput = getOutput("Exec");
        outValue = getOutput("Value");

        if (valueInput) {
            valueInput->setData(defaultValue);
        }

        if (outValue) {
            outValue->setComputeFunction([varName = this->variableName, defVal = this->defaultValue]() -> VariableValue {
                return VariableNode::getStoredValue(varName, defVal);
                });
        }
    }
}
