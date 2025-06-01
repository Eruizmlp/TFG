#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class RandomNode : public GraphNode {
    public:
        RandomNode(const std::string& name);

        void execute() override;

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
