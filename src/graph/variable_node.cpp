#include "variable_node.h"
#include <iostream>

namespace GraphSystem {

    std::unordered_map<std::string, VariableValue> VariableNode::variableStore;

    VariableNode::VariableNode(const std::string& name, const std::string& varName, const VariableValue& initialValue)
        : GraphNode(name, NodeCategory::DATA), variableName(varName), defaultValue(initialValue)
    {
        IOType ioType = IOType::FLOAT;

        // Detect initial value type
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

        if (!variableStore.count(variableName))
            variableStore[variableName] = defaultValue;

        outValue->setComputeFunction([this]() -> VariableValue {
            auto it = variableStore.find(variableName);
            if (it != variableStore.end()) {
                return it->second;
            }
            return 0.0f; 
            });

        outValue->setData(defaultValue);
    }

    void VariableNode::setupIO(IOType type) {
        execInput = addInput("Execute", IOType::EXECUTION);
        valueInput = addInput("Value", type);     
        execOutput = addOutput("Exec", IOType::EXECUTION);
        outValue = addOutput("Value", type);      
    }

    void VariableNode::setVariableName(const std::string& varName) {
        variableName = varName;
    }

    const std::string& VariableNode::getVariableName() const {
        return variableName;
    }

    void VariableNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        if (valueInput && valueInput->hasData()) {
            VariableValue value;

            switch (valueInput->getType()) {
            case IOType::FLOAT:
                value = valueInput->getFloat();
                break;
            case IOType::INT:
                value = valueInput->getInt();
                break;
            case IOType::BOOL:
                value = valueInput->getBool();
                break;
            case IOType::STRING:
                value = valueInput->getString();
                break;
            case IOType::VEC2:
                value = valueInput->getVec2();
                break;
            case IOType::VEC3:
                value = valueInput->getVec3();
                break;
            case IOType::VEC4:
                value = valueInput->getVec4();
                break;
            case IOType::MAT4:
                value = valueInput->getMat4();
                break;
            case IOType::MESH:
                value = valueInput->getMesh();
                break;
            default:
                std::cerr << "[VariableNode] Unsupported type in execute()\n";
                break;
            }

            variableStore[variableName] = value;
        }

        for (auto& link : execOutput->getLinks()) {
            if (auto* next = link->getTargetNode()) {
                next->setExecutionPending(true);
            }
        }
    }

    VariableValue VariableNode::getStoredValue(const std::string& varName) {
        auto it = variableStore.find(varName);
        return (it != variableStore.end()) ? it->second : 0.0f;
    }

    void VariableNode::setStoredValue(const std::string& varName, const VariableValue& value) {
        variableStore[varName] = value;
    }

} // namespace GraphSystem
