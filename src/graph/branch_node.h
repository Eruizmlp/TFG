#pragma once

#include "graph_node.h"

namespace GraphSystem {

    // Nodo condicional: recibe un bool en "Condition" y dispara "True" o "False".
    class BranchNode : public GraphNode {
    private:
        Input* execInput = nullptr; 
        Input* conditionInput = nullptr; 
        Output* trueOutput = nullptr; 
        Output* falseOutput = nullptr; 

    public:
        explicit BranchNode(const std::string& name);

        void execute() override;
    };

} 
