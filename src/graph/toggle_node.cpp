#include "toggle_node.h"

namespace GraphSystem {

    ToggleNode::ToggleNode(const std::string& name)
        : GraphNode(name)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        execOutput = addOutput("Exec", IOType::EXECUTION);
        stateOutput = addOutput("State", IOType::BOOL);

        stateOutput->setData(currentState);
    }

    void ToggleNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        currentState = !currentState;
        stateOutput->setData(currentState);

        for (auto& link : execOutput->getLinks()) {
            if (auto* next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }

}
