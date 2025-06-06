#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"
#include <queue>

namespace GraphSystem {
    class TranslateNode : public GraphNode {
    private:
        MeshInstance3D* targetMesh = nullptr;
        glm::vec3 offset;

        Input* execInput = nullptr;
        Input* transformInput = nullptr;
        Output* execOutput = nullptr;
        Input* offsetInput = nullptr;
        Output* transformOutput = nullptr;

    public:

        explicit TranslateNode(const std::string& name,
            const glm::vec3& offset = glm::vec3(0.0f));

        void setOffset(const glm::vec3& offset);
        glm::vec3 getOffset() const;

        void execute(std::queue<GraphNode*>& executionQueue) override;

    };
}
