#include "get_variable_node.h"
#include "variable_node.h"

namespace GraphSystem {

    GetVariableNode::GetVariableNode(const std::string& name, const std::string& varName)
        : GraphNode(name, NodeCategory::DATA), variableName(varName)
    {
        addInput("Execute", IOType::EXECUTION);
        outValue = addOutput("Value", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);
    }

    void GetVariableNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float val = VariableNode::getStoredValue(variableName);
        outValue->setData(val);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }

}
