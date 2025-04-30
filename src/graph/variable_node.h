#pragma once

#include "graph_node.h"

namespace GraphSystem {

    // Nodo que almacena un float. Al ejecutarse:
    // - Si llega un nuevo dato en "Value", lo guarda.
    // - Siempre emite el valor actual por "Value".
    class VariableNode : public GraphNode {
    private:
        Input* execInput = nullptr;  // "Execute"
        Input* inValue = nullptr;  // "Value"
        Output* execOutput = nullptr;  // "Exec"
        Output* outValue = nullptr;  // "Value"
        float   storedValue;            // valor interno

    public:
        explicit VariableNode(const std::string& name, float initialValue = 0.0f);

        void execute() override;
        float getValue() const { return storedValue; }
        void  setInitialValue(float v) { storedValue = v; }
    };

} // namespace GraphSystem
