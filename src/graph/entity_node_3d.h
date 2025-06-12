#pragma once
#include "graph_node.h"
#include "io.h"
#include "framework/nodes/mesh_instance_3d.h"
#include <queue>

namespace GraphSystem {

    class EntityNode3D : public GraphNode {
        MeshInstance3D* entity = nullptr;

        Output* outputEntity = nullptr;
        
    public:
        explicit EntityNode3D(const std::string& name);

        void setEntity(MeshInstance3D* e);
        MeshInstance3D* getEntity() const;


        void execute(std::queue<GraphNode*>& executionQueue) override;

    };

}
