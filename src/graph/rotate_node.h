#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {
    class RotateNode : public GraphNode {
    private:
        MeshInstance3D* targetMesh = nullptr;
        float angle = 30.0f;
        glm::vec3 axis;
        Input* execInput = nullptr;
        Input* transformInput = nullptr;
        Output* execOutput = nullptr;
        Input* angleInput = nullptr;
        Output* transformOutput = nullptr;

    public:
        explicit RotateNode(const std::string& name,
            float angle = 30.0f,
            glm::vec3 axis = glm::vec3(0, 1, 0));

        void setTarget(MeshInstance3D* mesh);
        void setRotationAngle(float angle);
        void setRotationAxis(const glm::vec3& axis);
        float getRotationAngle();
        void execute() override;
    };
}
