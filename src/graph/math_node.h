#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class MathNode : public GraphNode {
    private:
        Input* aInput = nullptr;
        Input* bInput = nullptr;
        Output* resultOutput = nullptr;
        char operation;

        float aValue = 1.0f;  
        float bValue = 1.0f;  

    public:
        MathNode(const std::string& name, char op = '+');

        void setOperation(char op);
    };

}
