#include "toggle_node.h"

namespace GraphSystem {

    ToggleNode::ToggleNode(const std::string& name)
        : GraphNode(name, NodeCategory::LOGIC)
    {
        execInput = addInput("Execute", IOType::EXECUTION); 
        execOutput = addOutput("Exec", IOType::EXECUTION); 
        stateOutput = addOutput("State", IOType::BOOL); 

        stateOutput->setData(VariableValue(currentState));  
    }

    void ToggleNode::execute() {
        if (!isExecutionPending()) return;  
        setExecutionPending(false); 

        currentState = !currentState;  
        stateOutput->setData(VariableValue(currentState));

        for (auto& link : execOutput->getLinks()) {  
            if (auto* next = link->getTargetNode())
                next->setExecutionPending(true); 
        }
    }

}
