#include "compare_node.h"

namespace GraphSystem {

    CompareNode::CompareNode(const std::string& name, CompareOp op)
        : GraphNode(name, NodeCategory::LOGIC), operation(op)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::BOOL);

        resultOutput->setComputeFunction([this]() -> bool {
            float a = aInput->hasData() ? aInput->getFloat() : 0.0f;
            float b = bInput->hasData() ? bInput->getFloat() : 0.0f;

            switch (operation) {
            case CompareOp::EQUAL:         return a == b;
            case CompareOp::NOT_EQUAL:     return a != b;
            case CompareOp::LESS:          return a < b;
            case CompareOp::GREATER:       return a > b;
            case CompareOp::LESS_EQUAL:    return a <= b;
            case CompareOp::GREATER_EQUAL: return a >= b;
            default:                       return false;
            }
            });

        resultOutput->setData(false);  
    }

    void CompareNode::setOperation(CompareOp op) {
        operation = op;
    }

    CompareOp CompareNode::getOperation() const {
        return operation;
    }

    void CompareNode::execute()
    {
    }

}
