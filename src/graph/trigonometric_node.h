#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class TrigonometricNode : public GraphNode {
    public:
        TrigonometricNode(const std::string& name, char op = 's');
        void setOperation(char op); // 's' = sin, 'c' = cos, 't' = tan

        void execute(std::queue<GraphNode*>& executionQueue) override;

    private:

        Input* angleInput;
        Output* resultOutput;

        char operation;
        float defaultAngle;
    };
}
