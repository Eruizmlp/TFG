#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {
    class RotateNode : public GraphNode {
    private:
        MeshInstance3D* targetMesh;
        float angle;  // Rotation angle in degrees
        glm::vec3 axis;
        Output* execOutput;
        Output* transformOutput;

    public:
        explicit RotateNode(const std::string& name,
            float angle = 30.0f,  // Default 30 degree rotation
            glm::vec3 axis = glm::vec3(0, 1, 0));  // Default Y-axis

        void setTarget(MeshInstance3D* mesh);
        void setRotationAngle(float angle);  // Renamed from setRotationSpeed
        void setRotationAxis(const glm::vec3& axis);
        void execute() override;
    };
}
