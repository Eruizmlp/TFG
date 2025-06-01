#pragma once
#include "graph_node.h"
#include <string>

namespace GraphSystem {

    enum class CompareOp {
        EQUAL, NOT_EQUAL,
        LESS, GREATER,
        LESS_EQUAL, GREATER_EQUAL
    };

    class CompareNode : public GraphNode {
    public:
        CompareNode(const std::string& name, CompareOp op = CompareOp::EQUAL);

        void setOperation(CompareOp op);
        CompareOp getOperation() const;

    private:
        Input* aInput;
        Input* bInput;
        Output* resultOutput;

        CompareOp operation;
    };

}
