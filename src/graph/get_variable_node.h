#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class GetVariableNode : public GraphNode {
    public:
        GetVariableNode(const std::string& name, const std::string& varName);

        void execute() override;

    private:
        Output* execOutput;
        Output* outValue;

        std::string variableName;
    };

}
