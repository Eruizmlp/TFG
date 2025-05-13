#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {
    class TranslateNode : public GraphNode {
    private:
        MeshInstance3D* targetMesh = nullptr;
        glm::vec3 offset;

        Input* execInput = nullptr;
        Output* execOutput = nullptr;
        Input* offsetInput = nullptr;
        Output* transformOutput = nullptr;

    public:

        explicit TranslateNode(const std::string& name,
            const glm::vec3& offset = glm::vec3(0.0f));

        void setTarget(MeshInstance3D* mesh);
        void setOffset(const glm::vec3& offset);
        glm::vec3 getOffset() const;

        void execute() override;
    };
}
