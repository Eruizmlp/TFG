#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream>

namespace GraphSystem {

    class TrigonometricNode : public GraphNode {
    public:
        explicit TrigonometricNode(const std::string& name, char op = 's');
        void setOperation(char op);

        void execute(std::queue<GraphNode*>& executionQueue) override;

       
        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:

        Input* angleInput = nullptr;
        Output* resultOutput = nullptr;

        char operation;
        float defaultAngle;
    };
}
