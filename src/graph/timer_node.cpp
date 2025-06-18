#include "timer_node.h"
#include <iostream>
#include <queue>

namespace GraphSystem {

    TimerNode::TimerNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        delayInput = addInput("Delay", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);
    }

    void TimerNode::execute(std::queue<GraphNode*>& executionQueue) {
        delayValue = 1.0f; 

        if (delayInput && delayInput->hasData()) {
            VariableValue v = delayInput->getValue();
            if (const float* pval = std::get_if<float>(&v)) {
                delayValue = *pval;
            }
            else {
                std::cerr << "[TimerNode] (" << getName() << ") Type mismatch for Delay input. Expected FLOAT. Using default 1.0f.\n";
            }
        }

        if (delayValue > 0.0f) {
            accumulatedTime = 0.0f;
            waiting = true;
            std::cout << "[TimerNode] (" << getName() << ") Timer started for " << delayValue << " seconds.\n";
        }
        else {
          
            
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }

    void TimerNode::update(float deltaTime) {
        if (!waiting) {
           
            return;
        }

        accumulatedTime += deltaTime;
        if (accumulatedTime >= delayValue) {
            waiting = false;
            std::cout << "[TimerNode][" << getName() << "] Timer FINISHED! Accumulated: " << accumulatedTime << "\n";
        }
    }


    bool TimerNode::isWaiting() const {
        return waiting;
    }
}
