#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class LerpNode : public GraphNode {
    public:
        LerpNode(const std::string& name);

        void execute() override;

    private:
        Input* aInput;
        Input* bInput;
        Input* tInput;

        Output* resultOutput;

        float defaultA = 0.0f;
        float defaultB = 1.0f;
        float defaultT = 0.5f;
    };

}
