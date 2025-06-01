#pragma once
#include "graph_node.h"

namespace GraphSystem {

    class LoopNode : public GraphNode {
    public:
        LoopNode(const std::string& name);

        void execute() override;
        void update(float deltaTime) override;

    private:
        Input* execInput = nullptr;
        Input* countInput = nullptr;

        Output* loopExecOutput = nullptr;
        Output* completedOutput = nullptr;
        Output* indexOutput = nullptr;

        int repeatCount = 1;   
        int currentIndex = 0;  
        bool looping = false;  
    };

}
