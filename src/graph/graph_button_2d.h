#pragma once

#include "framework/nodes/button_2d.h"
#include "graph.h"

namespace GraphSystem {
    class GraphButton2D : public ui::TextureButton2D {
    private:
        Graph* graph;

    public:
        GraphButton2D(
            const std::string& signal,
            const ui::sButtonDescription& desc,
            Graph* graph
        );

        bool on_pressed() override;
    };
}
