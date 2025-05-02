#pragma once

#include "graph_node.h"

namespace GraphSystem {

    class MathNode : public GraphNode {
    private:
        float aValue = 0.0f;
        float bValue = 0.0f;

        Input* execInput = nullptr;  
        Input* aInput = nullptr;  
        Input* bInput = nullptr;  
        Output* execOutput = nullptr;  
        Output* resultOutput = nullptr;  
        char    operation;                

    public:
        float getA() const { return aValue; }
        float getB() const { return bValue; }
        void  setA(float v) { aValue = v; }
        void  setB(float v) { bValue = v; }


        explicit MathNode(const std::string& name, char op = '+');

        void setOperation(char op);

        void execute() override;
    };

} 
