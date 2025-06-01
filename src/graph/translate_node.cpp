#include "translate_node.h"

namespace GraphSystem {

    TranslateNode::TranslateNode(const std::string& name,
        const glm::vec3& offset)
        : GraphNode(name, NodeCategory::TRANSFORM),

        execInput(addInput("Execute", IOType::EXECUTION)),
        execOutput(addOutput("Exec", IOType::EXECUTION)),

        transformInput(addInput("Transform", IOType::MESH)),
        offsetInput(addInput("Offset", IOType::VEC3)),
        transformOutput(addOutput("Transform", IOType::MESH)),

        offset(offset)
    {
        // seed the input socket with the default offset
        offsetInput->setData<glm::vec3>(offset);
    }

    void TranslateNode::setTarget(MeshInstance3D* mesh) {
        targetMesh = mesh;
    }

    void TranslateNode::setOffset(const glm::vec3& o) {
        offset = o;
    }

    glm::vec3 TranslateNode::getOffset() const {
        return offset;
    }

    void TranslateNode::execute() {
        if (!isExecutionPending())
            return;

        if (!targetMesh) {
            if (transformInput) {
                auto* connected = transformInput->getConnectedOutput();
                if (connected && connected->hasData()) {
                    targetMesh = connected->getMesh();
                }
            }
        }

        if (!targetMesh)
            return;

        if (offsetInput) {
            auto* connected = offsetInput->getConnectedOutput();
            if (connected && connected->hasData()) {
                offset = connected->getVec3();
            }
        }

        Transform t = targetMesh->get_transform();
        t.set_position(offset);
        targetMesh->set_transform(t);

        transformOutput->setData(targetMesh);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }

        setExecutionPending(false);
    }



}
