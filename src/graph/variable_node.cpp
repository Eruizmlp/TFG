#include "variable_node.h"

namespace GraphSystem {

    std::unordered_map<std::string, float> VariableNode::variableStore;
    VariableNode::VariableNode(const std::string& name, const std::string& varName, float initialValue)
        : GraphNode(name, NodeCategory::DATA), variableName(varName), defaultValue(initialValue)
    {
        inValue = addInput("Value", IOType::FLOAT);
        outValue = addOutput("Value", IOType::FLOAT);

        if (!variableStore.count(variableName))
            variableStore[variableName] = defaultValue;

        if (isLazyType(outValue->getType())) {
            outValue->setComputeFunction([this]() -> float {
                float val;
                if (inValue->getConnectedOutput()) {
                    val = inValue->getFloat();
                }
                else {
                    val = defaultValue;
                }
                variableStore[variableName] = val;
                return val;
                });
        }

        outValue->setData(defaultValue);
    }


    float VariableNode::getStoredValue(const std::string& varName) {
        auto it = variableStore.find(varName);
        return (it != variableStore.end()) ? it->second : 0.0f;
    }

    void VariableNode::setStoredValue(const std::string& varName, float value) {
        variableStore[varName] = value;
    }

}
