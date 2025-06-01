#pragma once
#include "graph_node.h"
#include <unordered_map>

namespace GraphSystem {

    class VariableNode : public GraphNode {
    public:
        VariableNode(const std::string& name, const std::string& varName = "", float initialValue = 0.0f);

        void setVariableName(const std::string& varName);
        const std::string& getVariableName() const;

        void execute() override;

        static float getStoredValue(const std::string& varName);
        static void setStoredValue(const std::string& varName, float value);

    private:
        Input* execInput = nullptr;   
        Input* valueInput = nullptr;  
        Output* outValue = nullptr;   
        Output* execOutput = nullptr;

        std::string variableName;
        float defaultValue;

        static std::unordered_map<std::string, float> variableStore;
    };

}
