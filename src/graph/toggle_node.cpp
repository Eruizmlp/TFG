#include "toggle_node.h"
#include <queue>
#include <fstream>
#include <iostream>

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


    void ToggleNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        file.write(reinterpret_cast<const char*>(&currentState), sizeof(currentState));
    }

    void ToggleNode::parse(std::ifstream& file) {
        GraphNode::parse(file);

        file.read(reinterpret_cast<char*>(&currentState), sizeof(currentState));
    }

    void ToggleNode::rebindPins() {

        execInput = getInput("Execute");
        execOutput = getOutput("Exec");
        stateOutput = getOutput("State");

        if (stateOutput) {
            stateOutput->setData(currentState);
        }
    }
}
