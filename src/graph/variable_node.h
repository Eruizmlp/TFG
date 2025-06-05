#pragma once

#include "graph_node.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <variant>

namespace GraphSystem {

    using VariableValue = std::variant<
        bool, int, float, std::string,
        glm::vec2, glm::vec3, glm::vec4, glm::mat4,
        MeshInstance3D*
    >;

    class VariableNode : public GraphNode {
    public:
        explicit VariableNode(const std::string& name, const std::string& varName = "", const VariableValue& initialValue = 0.0f);

        void setVariableName(const std::string& varName);
        const std::string& getVariableName() const;

        void execute() override;

        static VariableValue getStoredValue(const std::string& varName);
        static void setStoredValue(const std::string& varName, const VariableValue& value);

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

} // namespace GraphSystem
