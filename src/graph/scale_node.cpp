#include "scale_node.h"
#include <iostream>

namespace GraphSystem {

    ScaleNode::ScaleNode(const std::string& name, float factor)
        : GraphNode(name, NodeCategory::TRANSFORM),

        execInput(addInput("Execute", IOType::EXECUTION)),
        transformInput(addInput("Transform", IOType::MESH)),
        execOutput(addOutput("Exec", IOType::EXECUTION)),

        factorInput(addInput("Factor", IOType::FLOAT)),
        transformOutput(addOutput("Transform", IOType::MESH)),

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
        if (!isExecutionPending())
            return;

        if (!targetMesh) {
            if (transformInput && transformInput->hasData()) {
                targetMesh = transformInput->getMesh();
            }
        }

        if (!targetMesh) return;

        if (factorInput && factorInput->hasData()) {
            factor = factorInput->getFloat();
           
            factor = glm::clamp(std::abs(factor), 0.01f, 10.0f); 
        }

        Transform t = targetMesh->get_transform();
        t.set_scale(glm::vec3(factor)); 
        targetMesh->set_transform(t);

        transformOutput->setData(targetMesh);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }
        setExecutionPending(false);
    }



}
