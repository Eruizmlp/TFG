#include "timer_node.h"
#include "engine/engine.h"

namespace GraphSystem {

    TimerNode::TimerNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        delayInput = addInput("Delay", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
    }

    void TimerNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float delay = delayInput->hasData() ? delayInput->getFloat() : delayValue;
        if (delay <= 0.0f) delay = 0.001f; 

        delayValue = delay;
        accumulatedTime = 0.0f;
        waiting = true;
    }

    void TimerNode::update(float deltaTime) {
        if (!waiting) return;

        accumulatedTime += deltaTime;
        if (accumulatedTime >= delayValue) {
            waiting = false;

            for (auto& link : execOutput->getLinks()) {
                if (auto* next = link->getTargetNode()) {
                    next->setExecutionPending(true);
                }
            }
        }
    }

}
