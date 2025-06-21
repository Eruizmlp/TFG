#pragma once

#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class BranchNode : public GraphNode {
    private:
        Input* conditionInput = nullptr;
        Output* trueOutput = nullptr;
        Output* falseOutput = nullptr;

    public:
        explicit BranchNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void parse(std::ifstream& file) override;

        void serialize(std::ofstream& file) override;

        void rebindPins() override;
    };
}
