#include "variable_node.h"

namespace GraphSystem {

    VariableNode::VariableNode(const std::string& name, float initialValue)
        : GraphNode(name), storedValue(initialValue)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        inValue = addInput("Value", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);
        outValue = addOutput("Value", IOType::FLOAT);
    }

    void VariableNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        if (inValue->hasData()) {
            storedValue = inValue->getFloat();
        }

        outValue->setData(storedValue);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }

} 
