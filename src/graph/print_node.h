#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class PrintNode : public GraphNode {
    private:
        Input* execInput = nullptr;
        Output* execOutput = nullptr;

        Input* messageInput = nullptr;

        std::string defaultMessage = "Hello from PrintNode!";

    public:
        explicit PrintNode(const std::string& name);
        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;
    };

} // namespace GraphSystem
