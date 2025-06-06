#include "toggle_node.h"
#include <queue>

namespace GraphSystem {

    ToggleNode::ToggleNode(const std::string& name)
        : GraphNode(name, NodeCategory::LOGIC)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        execOutput = addOutput("Exec", IOType::EXECUTION);
        stateOutput = addOutput("State", IOType::BOOL);

        stateOutput->setData(VariableValue(currentState));
    }

    void ToggleNode::execute(std::queue<GraphNode*>& executionQueue) {

        currentState = !currentState;
        stateOutput->setData(VariableValue(currentState));

        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }
}
