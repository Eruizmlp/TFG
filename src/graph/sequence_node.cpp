// sequence_node.cpp
#include "sequence_node.h"

namespace GraphSystem {
    SequenceNode::SequenceNode(const std::string& name, int numOutputs)
        : GraphNode(name), numSteps(numOutputs) {
        addInput("Execute", IOType::EXECUTION);
        for (int i = 0; i < numOutputs; i++) {
            addOutput("Step" + std::to_string(i + 1), IOType::EXECUTION);
        }
    }

    void SequenceNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        std::string currentOutput = "Step" + std::to_string(currentStep + 1);
        for (auto* output : getOutputs()) {
            if (output->getName() == currentOutput) {
                for (auto* link : output->getLinks()) {
                    if (auto target = link->getTargetNode()) {
                        target->setExecutionPending(true);
                    }
                }
                break;
            }
        }
        currentStep = (currentStep + 1) % numSteps;
    }
}
