#pragma once

#include "node_widget_2d.h"
#include "entity_node_3d.h"
#include "context_menu.h"
#include "engine/sample_engine.h"

namespace GraphSystem {

    class EntityNodeWidget2D : public NodeWidget2D {
    private:
        EntityNode3D* entityNode;

    public:
        EntityNodeWidget2D(EntityNode3D* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void on_right_click() override;

    private:
        void show_entity_selector(); 
    };

} 
