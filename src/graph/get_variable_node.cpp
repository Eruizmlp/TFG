#include "get_variable_node.h"
#include "variable_node.h" 

namespace GraphSystem {

    GetVariableNode::GetVariableNode(const std::string& name, const std::string& varName)
        : GraphNode(name, NodeCategory::DATA), variableName(varName)
    {
        outValue = addOutput("Value", IOType::FLOAT);

        outValue->setComputeFunction([varName = this->variableName]() -> VariableValue {
            return VariableNode::getStoredValue(varName, 0.0f);
            });

        outValue->setData(VariableNode::getStoredValue(variableName, 0.0f));
    }

    void GetVariableNode::execute(std::queue<GraphNode*>& executionQueue)
    {
    }




}
