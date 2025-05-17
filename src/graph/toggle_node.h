#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class ToggleNode : public GraphNode {
    public:
        ToggleNode(const std::string& name);

        void execute() override;

    private:
        Input* execInput;
        Output* execOutput;
        Output* stateOutput;

        bool currentState = false;
    };

}
