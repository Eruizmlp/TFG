#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream> 

namespace GraphSystem {

    class LoopNode : public GraphNode {
    public:
        explicit LoopNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;
        void update(float deltaTime) override;

       
        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:

        Input* execInput = nullptr;
        Input* countInput = nullptr;
        Output* loopExecOutput = nullptr;
        Output* completedOutput = nullptr;
        Output* indexOutput = nullptr;

    };

}
