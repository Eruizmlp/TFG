#pragma once
#include "graph_node.h"
#include "io.h"
#include <queue>

namespace GraphSystem {

    class MathNode : public GraphNode {
    private:
        Input* aInput = nullptr;
        Input* bInput = nullptr;
        Output* resultOutput = nullptr;
        char operation;

    public:
        MathNode(const std::string& name, char op = '+');

        void execute(std::queue<GraphNode*>& executionQueue) override;


        void setOperation(char op);
    };
}
