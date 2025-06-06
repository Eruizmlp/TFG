#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class ToggleNode : public GraphNode {
    public:
        ToggleNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;


    private:
        Input* execInput = nullptr;
        Output* execOutput = nullptr;
        Output* stateOutput = nullptr;

        bool currentState = false;
    };

}
