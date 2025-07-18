#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"
#include <queue>

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

        void setRotationAngle(float angle);
        void setRotationAxis(const glm::vec3& axis);
        float getRotationAngle();
        const glm::vec3& getRotationAxis() const;

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;

        void rebindPins() override;
    };
}
