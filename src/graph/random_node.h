#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream>

namespace GraphSystem {

    class RandomNode : public GraphNode {
    public:
        explicit RandomNode(const std::string& name);
        void execute(std::queue<GraphNode*>& executionQueue) override;

  
        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
   
        Input* execInput = nullptr;
        Input* minInput = nullptr;
        Input* maxInput = nullptr;
        Output* execOutput = nullptr;
        Output* resultOutput = nullptr;


        float defaultMin = 0.0f;
        float defaultMax = 1.0f;
    };
}
