#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class TimerNode : public GraphNode {
    public:
        TimerNode(const std::string& name);

        void execute() override;
        void update(float deltaTime) override;

    private:
        Input* execInput;
        Input* delayInput;

        Output* execOutput;

        float delayValue = 1.0f;
        float accumulatedTime = 0.0f;
        bool waiting = false;
    };

}
