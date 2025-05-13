#include "scale_node.h"

namespace GraphSystem {

    ScaleNode::ScaleNode(const std::string& name, float factor)
        : GraphNode(name),

        execInput(addInput("Execute", IOType::EXECUTION)),
        execOutput(addOutput("Exec", IOType::EXECUTION)),

        factorInput(addInput("Factor", IOType::FLOAT)),
        transformOutput(addOutput("Transform", IOType::TRANSFORM)),

        factor(factor)
    {
        factorInput->setData<float>(factor);
    }

    void ScaleNode::setTarget(MeshInstance3D* mesh) {
        targetMesh = mesh;
    }

    void ScaleNode::setScaleFactor(float f) {
        factor = f;
    }

    float ScaleNode::getScaleFactor() const {
        return factor;
    }

    void ScaleNode::execute() {
        
        if (!isExecutionPending() || !targetMesh)
            return;

        if (factorInput && factorInput->hasData()) {
            factor = factorInput->getFloat();
        }

        targetMesh->scale(glm::vec3(factor));

        transformOutput->setData(targetMesh->get_transform());

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }

        setExecutionPending(false);
    }

}
