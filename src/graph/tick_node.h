#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class TickNode : public GraphNode {
    private:
        Input* startInput = nullptr;
        Input* stopInput = nullptr;
        Output* tickOutput = nullptr;
        bool running = false;

    public:
        explicit TickNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;
        void update(float dt) override;


        void start();
        void stop();
        bool isRunning() const;
    };
}
