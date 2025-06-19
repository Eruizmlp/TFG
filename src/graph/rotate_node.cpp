#include "rotate_node.h"
#include <cmath>
#include <iostream>
#include <queue>

namespace GraphSystem {

    // El constructor y los setters/getters que tenías están perfectos.
    RotateNode::RotateNode(const std::string& name, float angle_deg, glm::vec3 ax)
        : GraphNode(name, NodeCategory::TRANSFORM),
        angle(angle_deg),
        axis(glm::normalize(ax))
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        transformInput = addInput("Transform", IOType::MESH);
        angleInput = addInput("Angle", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        transformOutput = addOutput("Transform", IOType::MESH);

        angleInput->setData(VariableValue(this->angle));
        transformOutput->setData(VariableValue(static_cast<MeshInstance3D*>(nullptr)));
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
        if (angleInput && angleInput->hasData()) {
            VariableValue v = angleInput->getValue();
            if (const float* pval = std::get_if<float>(&v)) {
                return *pval;
            }
        }
        return angle;
    }

    const glm::vec3& RotateNode::getRotationAxis() const {
        return axis;
    }

    void RotateNode::execute(std::queue<GraphNode*>& executionQueue) {

        MeshInstance3D* currentMesh = nullptr;
        if (transformInput && transformInput->hasData()) {
            VariableValue v = transformInput->getValue();

            if (auto pval = std::get_if<MeshInstance3D*>(&v)) {
                currentMesh = *pval;
            }
            else {
                std::cerr << "[RotateNode] (" << getName() << ") Type mismatch en Transform. Se esperaba MESH.\n";
            }
        }

        
        if (currentMesh) {
            float currentAngle = getRotationAngle(); 

            glm::quat absoluteRotation = glm::angleAxis(glm::radians(currentAngle), glm::normalize(axis));
            Transform t = currentMesh->get_transform();
            t.set_rotation(glm::normalize(absoluteRotation));
            currentMesh->set_transform(t);

            transformOutput->setData(VariableValue(currentMesh));
        }
        else {

            std::cerr << "[RotateNode] (" << getName() << ") No hay malla de destino para rotar.\n";
        }

        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }
}
