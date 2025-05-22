#include "translate_node.h"

namespace GraphSystem {

    TranslateNode::TranslateNode(const std::string& name,
        const glm::vec3& offset)
        : GraphNode(name, NodeCategory::TRANSFORM),

        execInput(addInput("Execute", IOType::EXECUTION)),
        execOutput(addOutput("Exec", IOType::EXECUTION)),

        offsetInput(addInput("Offset", IOType::VEC3)),
        transformOutput(addOutput("Transform", IOType::TRANSFORM)),

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
        // only proceed if triggered and a target is set
        if (!isExecutionPending() || !targetMesh)
            return;

        
        if (offsetInput && offsetInput->hasData()) {
            offset = offsetInput->getVec3();
        }

        targetMesh->translate(offset);

        transformOutput->setData(targetMesh->get_transform());

      
        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
        setExecutionPending(false);
    }

}
