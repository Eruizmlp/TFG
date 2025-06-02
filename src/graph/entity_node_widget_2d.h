#pragma once

#include "node_widget_2d.h"
#include "entity_node_3d.h"
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {

    class EntityNodeWidget2D : public NodeWidget2D {
    private:
        EntityNode3D* entityNode = nullptr;
        MeshInstance3D* entity = nullptr;

    public:
        EntityNodeWidget2D(const std::string& nodeType,
            EntityNode3D* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
        void setEntity(MeshInstance3D* e);
    };

}
