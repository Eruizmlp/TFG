#pragma once

#include "framework/nodes/button_2d.h" 
#include "graph.h"
#include "graph_editor.h"

namespace GraphSystem {


    class GraphButton2D : public ui::TextureButton2D {
    private:

        Graph* graph;
        GraphEditor* editor;
        std::string node_to_create;

    public:
        GraphButton2D(
            const std::string& signal,
            const ui::sButtonDescription& desc,
            Graph* graph,
            GraphEditor* editor = nullptr,
            const std::string& nodeType = ""
        );

        bool on_pressed() override;
    };
}
