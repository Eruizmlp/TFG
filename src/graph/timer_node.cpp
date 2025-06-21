#include "timer_node.h"
#include <iostream>
#include <queue>
#include <fstream>

namespace GraphSystem {

    TimerNode::TimerNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        delayInput = addInput("Delay", IOType::FLOAT);
        execOutput = addOutput("Exec", IOType::EXECUTION);

        delayInput->setData(defaultDelay);
    }


    void TimerNode::execute(std::queue<GraphNode*>& executionQueue) {

        delayValue = delayInput->getFloat();

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

 

    void TimerNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        file.write(reinterpret_cast<const char*>(&defaultDelay), sizeof(defaultDelay));
        file.write(reinterpret_cast<const char*>(&delayValue), sizeof(delayValue));
        file.write(reinterpret_cast<const char*>(&accumulatedTime), sizeof(accumulatedTime));
        file.write(reinterpret_cast<const char*>(&waiting), sizeof(waiting));
    }

    void TimerNode::parse(std::ifstream& file) {
        GraphNode::parse(file);

        file.read(reinterpret_cast<char*>(&defaultDelay), sizeof(defaultDelay));
        file.read(reinterpret_cast<char*>(&delayValue), sizeof(delayValue));
        file.read(reinterpret_cast<char*>(&accumulatedTime), sizeof(accumulatedTime));
        file.read(reinterpret_cast<char*>(&waiting), sizeof(waiting));
    }

    void TimerNode::rebindPins() {

        execInput = getInput("Execute");
        delayInput = getInput("Delay");
        execOutput = getOutput("Exec");


        if (delayInput) {
            delayInput->setData(defaultDelay);
        }
    }
}
