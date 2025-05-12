// visual_link.h
#pragma once

#include "framework/nodes/node_2d.h"
#include "framework/nodes/panel_2d.h"
#include "graph.h"
#include <vector>

namespace GraphSystem {

    class PortButton2D;
    class GraphEditor;

    class VisualLink2D : public Node2D {
    public:
        VisualLink2D(Graph* graph, GraphEditor* editor, float thickness = 4.0f);
        ~VisualLink2D() override;

        // Called every frame to update link visuals
        void update(float delta_time) override;

    private:
        void clearLinks();
        void buildLinks();

        Graph* graph;
        GraphEditor* editor;
        float linkThickness;
        std::vector<ui::XRPanel*> panels;
    };

}
