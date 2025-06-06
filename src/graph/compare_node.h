#pragma once
#include "graph_node.h"
#include <string>
#include <queue>

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

        void execute(std::queue<GraphNode*>& executionQueue) override;


    private:
        Input* aInput;
        Input* bInput;
        Output* resultOutput;

        CompareOp operation;
    };

}
