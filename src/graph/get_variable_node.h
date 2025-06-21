#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class GetVariableNode : public GraphNode {
    public:
        explicit GetVariableNode(const std::string& name, const std::string& varName = "", IOType type = IOType::FLOAT);

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;


    private:
        Output* outValue;
        std::string variableName;

        IOType variableType;        
        VariableValue defaultValue; 
    };

}
