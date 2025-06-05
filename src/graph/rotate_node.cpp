#include "rotate_node.h"
#include <cmath> 
#include <iostream>

namespace GraphSystem {

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
        if (angleInput && angleInput->getConnectedOutput() && angleInput->getConnectedOutput()->hasData()) {
            return angleInput->getFloat();
        }
        return angle;
    }

    void RotateNode::execute() {

        if (!isExecutionPending()) {
            return;
        }

        if (!targetMesh) { 


            if (transformInput && transformInput->getConnectedOutput() && transformInput->getConnectedOutput()->hasData()) { // 
                targetMesh = transformInput->getConnectedOutput()->getMesh(); 
            }

            setExecutionPending(false); 
            return; 

        }

        float currentAngle = angle; 
        if (angleInput && angleInput->getConnectedOutput() && angleInput->getConnectedOutput()->hasData()) { 
            currentAngle = angleInput->getConnectedOutput()->getFloat();   
        }

        glm::quat absoluteRotation = glm::angleAxis(glm::radians(currentAngle), glm::normalize(axis)); 
        Transform t = targetMesh->get_transform(); 
        t.set_rotation(glm::normalize(absoluteRotation)); 

        targetMesh->set_transform(t); 

        transformOutput->setData(VariableValue(targetMesh)); 

        for (auto& link : execOutput->getLinks()) { 
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true); 
        }

        setExecutionPending(false); 
    }
}
