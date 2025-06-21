#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream>

namespace GraphSystem {

    class ToggleNode : public GraphNode {
    public:
        explicit ToggleNode(const std::string& name);
        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
        Input* execInput = nullptr;
        Output* execOutput = nullptr;
        Output* stateOutput = nullptr;

        bool currentState = false;
    };

}
