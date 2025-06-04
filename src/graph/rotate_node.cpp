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
        
        angle = std::fmod(a, 360.0f);
        if (angle < 0.0f) {
            angle += 360.0f;
        }
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
            if (transformInput) {
                auto* connected = transformInput->getConnectedOutput();
                if (connected && connected->hasData()) {
                    targetMesh = connected->getMesh();
                }
            }
        }

        if (!targetMesh)
            return;

        if (angleInput) {
            auto* connected = angleInput->getConnectedOutput();
            if (connected && connected->hasData()) {
                setRotationAngle(connected->getFloat()); 
            }
        }

        glm::quat absoluteRotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
        Transform t = targetMesh->get_transform();
        t.set_rotation(glm::normalize(absoluteRotation));

        targetMesh->set_transform(t);

        transformOutput->setData(targetMesh);

        for (auto& link : execOutput->getLinks()) {
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true);
        }

        setExecutionPending(false);
    }




}
