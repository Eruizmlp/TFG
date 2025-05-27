#pragma once
#include "node_widget_2d.h"
#include "graph/nodes/entity_node_3d.h"

class EntityNodeWidget2D : public NodeWidget2D {
public:
    EntityNodeWidget2D(GraphSystem::GraphNode* node);

    void on_right_click() override; // para mostrar el context menu
};
