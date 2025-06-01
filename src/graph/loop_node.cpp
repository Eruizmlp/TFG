#include "loop_node.h"

namespace GraphSystem {

    LoopNode::LoopNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        countInput = addInput("Count", IOType::FLOAT);

        loopExecOutput = addOutput("LoopExec", IOType::EXECUTION);
        completedOutput = addOutput("Completed", IOType::EXECUTION);
        indexOutput = addOutput("Index", IOType::FLOAT);
    }

    void LoopNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float countVal = countInput->hasData() ? countInput->getFloat() : static_cast<float>(repeatCount);
        repeatCount = static_cast<int>(countVal);

        currentIndex = 0;
        looping = (repeatCount > 0);
    }

    void LoopNode::update(float deltaTime) {
        if (!looping) return;

        if (currentIndex < repeatCount) {
            indexOutput->setData(static_cast<float>(currentIndex));

            for (auto& link : loopExecOutput->getLinks()) {
                if (auto* next = link->getTargetNode()) {
                    next->setExecutionPending(true);
                }
            }

            ++currentIndex;
        }
        else {
            looping = false;

            for (auto& link : completedOutput->getLinks()) {
                if (auto* next = link->getTargetNode()) {
                    next->setExecutionPending(true);
                }
            }
        }
    }

}
