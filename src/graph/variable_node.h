#pragma once

#include "graph_node.h"
#include "io.h" 
#include <queue>
#include <unordered_map>

namespace GraphSystem {


    class VariableNode : public GraphNode {
    public:
        explicit VariableNode(const std::string& name, const std::string& varName = "", const VariableValue& initialValue = 0.1f);

        void setVariableName(const std::string& varName);
        const std::string& getVariableName() const;

        void execute(std::queue<GraphNode*>& executionQueue) override;

        static void setStoredValue(const std::string& varName, const VariableValue& value);
        static VariableValue getStoredValue(const std::string& varName, const VariableValue& defaultValue);
        static const auto& getStore() { return variableStore; }
        static void clearStore() { variableStore.clear(); }

        void rebindPins() override;
        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;


    private:
        static std::unordered_map<std::string, VariableValue> variableStore;
        std::string variableName;
        VariableValue defaultValue;

        Input* valueInput = nullptr;
        Input* execInput = nullptr;
        Output* outValue = nullptr;
        Output* execOutput = nullptr;

        void setupIO(IOType type);
    };
}
