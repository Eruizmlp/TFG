// tick_node.h
#pragma once

#include "graph_node.h"

namespace GraphSystem {

    /// Emits a "Tick" Exec every frame once started; can be toggled off again.
    class TickNode : public GraphNode {
    private:
        Input* startInput = nullptr;
        Input* stopInput = nullptr;
        Output* tickOutput = nullptr;
        bool    running = false;

    public:
        explicit TickNode(const std::string& name);
        void execute() override;
        void update(float dt) override;
    };

} // namespace GraphSystem
