#pragma once

#include "graph_node.h"

namespace GraphSystem {


    class VariableNode : public GraphNode {
    private:
        Input* execInput = nullptr;  
        Input* inValue = nullptr;  
        Output* execOutput = nullptr;  
        Output* outValue = nullptr;  
        float   storedValue;           

    public:
        explicit VariableNode(const std::string& name, float initialValue = 0.0f);

        void execute() override;
        float getValue() const { return storedValue; }
        void  setInitialValue(float v) { storedValue = v; }
  
    };

} 
