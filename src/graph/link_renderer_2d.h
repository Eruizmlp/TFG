#pragma once

#include "framework/nodes/node_2d.h"
#include "framework/nodes/panel_2d.h"
#include "graph.h"
#include <vector>
#include "link_quad.h"

namespace GraphSystem {

    class PortButton2D;
    class GraphEditor;

    class LinkRenderer2D : public Node2D {
    public:
        LinkRenderer2D(Graph* graph, GraphEditor* editor, float thickness = 4.0f);
        ~LinkRenderer2D() override;

        void update(float delta_time) override;

    private:
        void clearLinks();
        void buildLinks();

        Graph* graph;
        GraphEditor* editor;
        float linkThickness;
        std::vector<LinkQuad*> links;

    };

}
