#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class TrigonometricNode : public GraphNode {
    public:
        TrigonometricNode(const std::string& name, char op = 's'); 

        void setOperation(char op);  // 's' = sin, 'c' = cos, 't' = tan
        void execute() override;

    private:
        Input* execInput;
        Input* angleInput;
        Output* execOutput;
        Output* resultOutput;

        char operation;
        float defaultAngle;
    };

}
