#include "get_variable_node.h"
#include "variable_node.h"

namespace GraphSystem {

    GetVariableNode::GetVariableNode(const std::string& name, const std::string& varName)
        : GraphNode(name, NodeCategory::DATA), variableName(varName)
    {
        outValue = addOutput("Value", IOType::FLOAT);

        outValue->setComputeFunction([this]() -> float {
            return VariableNode::getStoredValue(variableName);
            });

        outValue->setData(VariableNode::getStoredValue(variableName));
    }

}
