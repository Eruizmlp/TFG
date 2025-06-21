#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream>

namespace GraphSystem {

    class TimerNode : public GraphNode {
    public:
        explicit TimerNode(const std::string& name);
        void execute(std::queue<GraphNode*>& executionQueue) override;
        bool isWaiting() const;
        void update(float deltaTime) override;


        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
        Input* execInput = nullptr;
        Input* delayInput = nullptr;
        Output* execOutput = nullptr;

        float defaultDelay = 1.0f;

        float delayValue = 1.0f;         
        float accumulatedTime = 0.0f;
        bool waiting = false;
    };

}
