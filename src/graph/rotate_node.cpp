#include "rotate_node.h"

namespace GraphSystem {

    RotateNode::RotateNode(const std::string& name, float angle, glm::vec3 axis)
        : GraphNode(name, NodeCategory::TRANSFORM),

        execInput(addInput("Execute", IOType::EXECUTION)),
        execOutput(addOutput("Exec", IOType::EXECUTION)),

        angleInput(addInput("Angle", IOType::FLOAT)),

        transformInput(addInput("Transform", IOType::MESH)),
        transformOutput(addOutput("Transform", IOType::MESH)),
        angle(angle),
        axis(glm::normalize(axis))
    {
        angleInput->setData<float>(angle);
    }

    void RotateNode::setTarget(MeshInstance3D* mesh) {
        targetMesh = mesh;
    }

    void RotateNode::setRotationAngle(float a) {
        angle = a;
    }

    void RotateNode::setRotationAxis(const glm::vec3& a) {
        axis = glm::normalize(a);
    }

    float RotateNode::getRotationAngle() {
        return angle;
    }

    void RotateNode::execute() {
        if (!isExecutionPending())
            return;

        if (!targetMesh) {   
            if (transformInput && transformInput->hasData()) {
                targetMesh = transformInput->getMesh();
            }
        }

        if (!targetMesh) return;  

        if (angleInput && angleInput->hasData()) {
            angle = angleInput->getFloat();
        }

        glm::quat current = targetMesh->get_rotation();
        glm::quat delta = glm::angleAxis(glm::radians(angle), axis);
        targetMesh->set_rotation(glm::normalize(delta * current));

        transformOutput->setData(targetMesh);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }

        setExecutionPending(false);
    }

}
