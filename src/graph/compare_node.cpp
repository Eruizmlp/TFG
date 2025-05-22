#include "compare_node.h"

namespace GraphSystem {

    CompareNode::CompareNode(const std::string& name, CompareOp op)
        : GraphNode(name, NodeCategory::LOGIC), operation(op)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::BOOL);
    }

    void CompareNode::setOperation(CompareOp op) {
        operation = op;
    }

    CompareOp CompareNode::getOperation() const {
        return operation;
    }

    void CompareNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float a = aInput->hasData() ? aInput->getFloat() : 0.0f;
        float b = bInput->hasData() ? bInput->getFloat() : 0.0f;
        bool result = false;

        switch (operation) {
        case CompareOp::EQUAL:         result = (a == b); break;
        case CompareOp::NOT_EQUAL:     result = (a != b); break;
        case CompareOp::LESS:          result = (a < b); break;
        case CompareOp::GREATER:       result = (a > b); break;
        case CompareOp::LESS_EQUAL:    result = (a <= b); break;
        case CompareOp::GREATER_EQUAL: result = (a >= b); break;
        }

        resultOutput->setData(result);
    }

}
