#pragma once
#include "graph_node.h"
#include <glm/glm.hpp>

namespace GraphSystem {

    enum class MathMode {
        FLOAT,
        VEC3
    };

    class MathNode : public GraphNode {
    private:
        Input* aInput = nullptr;
        Input* bInput = nullptr;
        Output* resultOutput = nullptr;

        char operation;
        MathMode mode = MathMode::FLOAT;

        float aValue = 1.0f;
        float bValue = 1.0f;
        glm::vec3 aVec3 = glm::vec3(1.0f);
        glm::vec3 bVec3 = glm::vec3(1.0f);

        void updateComputeFunction();  

    public:
        MathNode(const std::string& name, char op = '+');

        void execute() override {}

        void setOperation(char op);
        void setMode(MathMode newMode);
        MathMode getMode() const { return mode; }
    };

}
