#pragma once

#include "framework/nodes/button_2d.h"
#include "../graph/graph.h"
#include "../graph/graph_editor.h" 
#include <iostream>

namespace GraphSystem {

    class GraphButton2D : public ui::Button2D {
    private:
        GraphSystem::Graph* graph;
        GraphSystem::GraphEditor* editor = nullptr;
        std::string node_to_create;

    public:
        GraphButton2D(const std::string& signal,
            const ui::sButtonDescription& desc,
            Graph* graph,
            GraphEditor* editor = nullptr,
            const std::string& nodeType = "");

        void set_editor(GraphEditor* e) { editor = e; }
        void set_node_type_to_create(const std::string& type) { node_to_create = type; }

        bool on_pressed() override;
    };

} // namespace GraphSystem
