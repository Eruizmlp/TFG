#include "branch_node.h"
#include <iostream>

namespace GraphSystem {

    BranchNode::BranchNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        addInput("Execute", IOType::EXECUTION);
        conditionInput = addInput("Condition", IOType::BOOL);
        trueOutput = addOutput("True", IOType::EXECUTION);
        falseOutput = addOutput("False", IOType::EXECUTION);
    }

    void BranchNode::execute(std::queue<GraphNode*>& executionQueue) {

        bool condition = false;
        if (conditionInput && conditionInput->hasData()) {
            VariableValue val = conditionInput->getValue();
            if (const bool* p_val = std::get_if<bool>(&val)) {
                condition = *p_val;
            }
            else {
                std::cerr << "[BranchNode] (" << getName() << ") Type mismatch for Condition input. Expected BOOL.\n";
            }
        }

        Output* pinToFollow = condition ? trueOutput : falseOutput;

        if (pinToFollow) {
            for (auto* link : pinToFollow->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }

    void BranchNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
    }

    void BranchNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
    }

    void BranchNode::rebindPins() {
        conditionInput = getInput("Condition");
        trueOutput = getOutput("True");
        falseOutput = getOutput("False");
    }
}
