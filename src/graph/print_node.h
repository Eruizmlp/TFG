#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class PrintNode : public GraphNode {
    private:
        Input* execInput = nullptr;
        Output* execOutput = nullptr;

        Input* messageInput = nullptr;

    public:
        explicit PrintNode(const std::string& name);
        void execute(std::queue<GraphNode*>& executionQueue) override;


    };

} // namespace GraphSystem
