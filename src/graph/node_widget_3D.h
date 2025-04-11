#pragma once

#include "framework/nodes/node_3d.h"
#include "graph_node.h"
#include "framework/nodes/environment_3d.h"
#include "graphics/renderer_storage.h"
#include "graphics/material.h"
#include "framework/colors.h"


namespace GraphSystem {

    class NodeWidget3D : public Node3D {
    private:
        GraphNode* logic_node = nullptr;
        MeshInstance3D* visual = nullptr;

    public:
        NodeWidget3D(GraphNode* node);

        void set_position_world(const glm::vec3& pos);
        GraphNode* get_logic_node() const { return logic_node; }

        void highlight(bool enable);
    };

}
