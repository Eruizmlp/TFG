#include "set_variable_node.h"

namespace GraphSystem {

    SetVariableNode::SetVariableNode(const std::string& name, const std::string& variableName)
        : GraphNode(name, NodeCategory::FLOW), targetVariable(variableName)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        valueInput = addInput("Value", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);
    }

    void SetVariableNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        if (valueInput && valueInput->hasData()) {
            float value = valueInput->getFloat();
            VariableNode::setStoredValue(targetVariable, value); 
        }

        for (auto& link : execOutput->getLinks()) {
            if (auto* next = link->getTargetNode()) {
                next->setExecutionPending(true);
            }
        }
    }

}
