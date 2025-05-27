#pragma once
#include "graph_node.h"
#include "io.h"
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {

    class EntityNode3D : public GraphNode {
        MeshInstance3D* entity = nullptr;

        // Output references
        Output* outputMesh = nullptr;
        Output* outputTransform = nullptr;

    public:
        EntityNode3D();

        void setEntity(MeshInstance3D* e);
        MeshInstance3D* getEntity() const;

        void execute() override;
    };

}
