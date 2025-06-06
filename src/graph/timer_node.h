#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class TimerNode : public GraphNode {
    public:
        TimerNode(const std::string& name);
        void execute(std::queue<GraphNode*>& executionQueue) override;

        bool isWaiting() const;

        void update(float deltaTime) override;

    private:
        Input* execInput = nullptr;
        Input* delayInput = nullptr;

        Output* execOutput = nullptr;

        float delayValue = 1.0f;
        float accumulatedTime = 0.0f;
        bool waiting = false;
    };

}
