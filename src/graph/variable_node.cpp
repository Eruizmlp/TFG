#include "variable_node.h"

namespace GraphSystem {

    std::unordered_map<std::string, float> VariableNode::variableStore;

    VariableNode::VariableNode(const std::string& name, const std::string& varName, float initialValue)
        : GraphNode(name, NodeCategory::DATA), variableName(varName), defaultValue(initialValue)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        valueInput = addInput("Value", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);   
        outValue = addOutput("Value", IOType::FLOAT);

        if (!variableStore.count(variableName))
            variableStore[variableName] = defaultValue;

        if (outValue->getType() == IOType::FLOAT) {
            outValue->setComputeFunction([this]() -> float {
                return variableStore[variableName];
                });
        }

        outValue->setData(defaultValue);
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
            float value = valueInput->getFloat();
            variableStore[variableName] = value; 
        }

        for (auto& link : execOutput->getLinks()) {
            if (auto* next = link->getTargetNode()) {
                next->setExecutionPending(true);
            }
        }
    }


    float VariableNode::getStoredValue(const std::string& varName) {
        auto it = variableStore.find(varName);
        return (it != variableStore.end()) ? it->second : 0.0f;
    }

    void VariableNode::setStoredValue(const std::string& varName, float value) {
        variableStore[varName] = value;
    }

}
