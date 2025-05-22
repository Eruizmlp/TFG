#include "branch_node.h"

namespace GraphSystem {

    BranchNode::BranchNode(const std::string& name)
        : GraphNode(name, NodeCategory::LOGIC)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        conditionInput = addInput("Condition", IOType::BOOL);
        trueOutput = addOutput("True", IOType::EXECUTION);
        falseOutput = addOutput("False", IOType::EXECUTION);
    }

    void BranchNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        bool cond = conditionInput->hasData() ? conditionInput->getBool() : false;

        // Seleccionamos la salida
        Output* out = (cond ? trueOutput : falseOutput);
        for (auto& link : out->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }

} 
