#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {
    class ScaleNode : public GraphNode {
    private:
        MeshInstance3D* targetMesh = nullptr;
        float factor = 1.0f;

        Input* execInput = nullptr;
        Input* transformInput = nullptr;
        Output* execOutput = nullptr;
        Input* factorInput = nullptr;
        Output* transformOutput = nullptr;

    public:

        explicit ScaleNode(const std::string& name,
            float factor = 1.0f);

        void setTarget(MeshInstance3D* mesh);
        void setScaleFactor(float f);
        float getScaleFactor() const;

        void execute() override;
    };
}
