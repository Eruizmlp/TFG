#pragma once

#include "graph_node.h"

namespace GraphSystem {


    class TickNode : public GraphNode {
    private:
        Input* startInput = nullptr;
        Input* stopInput = nullptr;
        Output* tickOutput = nullptr;
        bool running = false;

    public:
        explicit TickNode(const std::string& name);

        void execute() override;
        void update(float dt) override;
        bool isTickNode() const override { return true; }

        bool isRunning() const { return running; }
        void setRunning(bool r) { running = r; }
    };

} 
