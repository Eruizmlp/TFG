#pragma once
#include "graph_node.h"
#include <unordered_map>

namespace GraphSystem {

    class VariableNode : public GraphNode {
    public:
        VariableNode(const std::string& name, const std::string& varName = "", float initialValue = 5.0f);

        static float getStoredValue(const std::string& varName);
        static void setStoredValue(const std::string& varName, float value);

        void setVariableName(const std::string& name) { variableName = name; }

    private:
        Input* inValue;
        Output* outValue;

        std::string variableName;
        float defaultValue;

        static std::unordered_map<std::string, float> variableStore;
    };

}
