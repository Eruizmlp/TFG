#pragma once
#include "graph_node.h"
#include <fstream> 

namespace GraphSystem {

    class LerpNode : public GraphNode {
    public:
        explicit LerpNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
        
        Input* aInput = nullptr;
        Input* bInput = nullptr;
        Input* tInput = nullptr;
        Output* resultOutput = nullptr;

        float defaultA = 0.0f;
        float defaultB = 1.0f;
        float defaultT = 0.5f;
    };

}
