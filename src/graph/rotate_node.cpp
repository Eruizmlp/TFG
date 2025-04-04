#include "rotate_node.h"
#include <glm/gtc/matrix_transform.hpp>

namespace GraphSystem {

    RotateNode::RotateNode(const std::string& name, float angle, glm::vec3 axis)
        : GraphNode(name),
        angle(angle),  // Now storing angle instead of speed
        axis(glm::normalize(axis)),
        execOutput(addOutput("Exec", GraphSystem::IOType::EXECUTION)),
        transformOutput(addOutput("Transform", GraphSystem::IOType::TRANSFORM))
    {
    }

    void RotateNode::setTarget(MeshInstance3D* mesh) {
        targetMesh = mesh;
    }

    void RotateNode::setRotationAngle(float angle) {  // Renamed from setRotationSpeed
        this->angle = angle;
    }

    void RotateNode::setRotationAxis(const glm::vec3& axis) {
        this->axis = glm::normalize(axis);
    }

    void RotateNode::execute() {
        if (!targetMesh) return;

        // Apply single rotation
        glm::quat currentRotation = targetMesh->get_rotation();
        glm::quat deltaRotation = glm::angleAxis(glm::radians(angle), axis);
        targetMesh->set_rotation(glm::normalize(deltaRotation * currentRotation));

        // Output the new transform
        if (transformOutput) {
            transformOutput->setData(targetMesh->get_transform());
        }

        // Propagate execution
        if (execOutput) {
            for (auto& link : execOutput->getLinks()) {
                if (auto target = link->getTargetNode()) {
                    target->setExecutionPending(true);
                }
            }
        }
    }
}
