#pragma once

#include "graph_node.h"

namespace GraphSystem {

    class MathNode : public GraphNode {
    private:
        Input* execInput = nullptr;  
        Input* aInput = nullptr;  
        Input* bInput = nullptr;  
        Output* execOutput = nullptr;  
        Output* resultOutput = nullptr;  
        char    operation;                

    public:

        explicit MathNode(const std::string& name, char op = '+');

        void setOperation(char op);

        void execute() override;
    };

} 
