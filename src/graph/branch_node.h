#pragma once

#include "graph_node.h"

namespace GraphSystem {

    class BranchNode : public GraphNode {
    private:
        Input* execInput = nullptr; 
        Input* conditionInput = nullptr; 
        Output* trueOutput = nullptr; 
        Output* falseOutput = nullptr; 

    public:
        explicit BranchNode(const std::string& name);

        void execute() override;
    };

} 
