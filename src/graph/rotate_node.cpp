#include "rotate_node.h"
#include <glm/gtc/matrix_transform.hpp>

namespace GraphSystem {

    RotateNode::RotateNode(const std::string& name, float angle, glm::vec3 axis)
        : GraphNode(name),

        execInput(addInput("Execute", IOType::EXECUTION)),

        execOutput(addOutput("Exec", IOType::EXECUTION)),

        transformOutput(addOutput("Transform", IOType::TRANSFORM)),
        angle(angle),
        axis(glm::normalize(axis))
    {
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

    void RotateNode::execute() {

        if (!isExecutionPending() || !targetMesh) return;

        glm::quat current = targetMesh->get_rotation();
        glm::quat delta = glm::angleAxis(glm::radians(angle), axis);
        targetMesh->set_rotation(glm::normalize(delta * current));

        transformOutput->setData(targetMesh->get_transform());

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode()) {
                next->setExecutionPending(true);
            }
        }

        setExecutionPending(false);
    }

}
