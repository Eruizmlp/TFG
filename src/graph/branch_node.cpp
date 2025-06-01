#include "branch_node.h"

namespace GraphSystem {

    BranchNode::BranchNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        addInput("Execute", IOType::EXECUTION);   
        conditionInput = addInput("Condition", IOType::BOOL);  

        trueOutput = addOutput("True", IOType::EXECUTION);  
        falseOutput = addOutput("False", IOType::EXECUTION); 
    }


    void BranchNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        bool condition = false;
        if (conditionInput) {
            auto* connected = conditionInput->getConnectedOutput();
            if (connected && connected->hasData()) {
                condition = connected->getBool();  
            }
        }

        auto& outputLinks = condition ? trueOutput->getLinks() : falseOutput->getLinks();

        for (auto* link : outputLinks) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }


} 
