#pragma once
#include "graph_node.h"
#include <glm/glm.hpp>
#include <queue>
#include <fstream>

namespace GraphSystem {

    class SetColorNode : public GraphNode {
    public:
        explicit SetColorNode(const std::string& name);

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void setDefaultR(float r);
        void setDefaultG(float g);
        void setDefaultB(float b);
        void setDefaultA(float a);

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:

        Input* execInput = nullptr;
        Input* transformInput = nullptr;
        Output* execOutput = nullptr;
        Output* transformOutput = nullptr;


        Input* rInput = nullptr;
        Input* gInput = nullptr;
        Input* bInput = nullptr;
        Input* aInput = nullptr;

        float defaultR = 1.0f;
        float defaultG = 1.0f;
        float defaultB = 1.0f;
        float defaultA = 1.0f;
    };

}
