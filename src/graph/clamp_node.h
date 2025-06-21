#pragma once
#include "graph_node.h"
#include <queue>

namespace GraphSystem {

    class ClampNode : public GraphNode {
    public:
        ClampNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
        Input* valueInput;
        Input* minInput;
        Input* maxInput;

        Output* resultOutput;

        float defaultValue = 0.0f;
        float defaultMin = 0.0f;
        float defaultMax = 1.0f;
    };

}
