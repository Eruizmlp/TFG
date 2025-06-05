#include "scale_node.h"
#include <iostream>  
#include <glm/common.hpp> 
#include <cmath> 

namespace GraphSystem {

    ScaleNode::ScaleNode(const std::string& name, float fact) 
        : GraphNode(name, NodeCategory::TRANSFORM), 
        factor(fact) 
    {
        execInput = addInput("Execute", IOType::EXECUTION); 
        transformInput = addInput("Transform", IOType::MESH); 
        factorInput = addInput("Factor", IOType::FLOAT); 

        execOutput = addOutput("Exec", IOType::EXECUTION);  
        transformOutput = addOutput("Transform", IOType::MESH);  

        factorInput->setData(VariableValue(this->factor));  
        transformOutput->setData(VariableValue(static_cast<MeshInstance3D*>(nullptr)));
    }

    void ScaleNode::setTarget(MeshInstance3D* mesh) {
        targetMesh = mesh; 
    }

    void ScaleNode::setScaleFactor(float f) {
        factor = f;  
    }

    float ScaleNode::getScaleFactor() const { 
        
        if (factorInput && factorInput->getConnectedOutput() && factorInput->getConnectedOutput()->hasData()) {
            return factorInput->getFloat();
        }
        return factor;
    }

    void ScaleNode::execute() {
        if (!isExecutionPending()) return; 

        if (!targetMesh) { 
            if (transformInput && transformInput->getConnectedOutput() && transformInput->getConnectedOutput()->hasData()) {
                targetMesh = transformInput->getConnectedOutput()->getMesh();
            }
        }

        if (!targetMesh) { // 
            setExecutionPending(false);
            return;
        }

        float currentFactor = factor; 
        if (factorInput && factorInput->getConnectedOutput() && factorInput->getConnectedOutput()->hasData()) {
            currentFactor = factorInput->getConnectedOutput()->getFloat();
            currentFactor = glm::clamp(std::abs(currentFactor), 0.01f, 100.0f); 
        }
        else {

            currentFactor = glm::clamp(std::abs(currentFactor), 0.01f, 100.0f);
        }


        Transform t = targetMesh->get_transform(); 
        t.set_scale(glm::vec3(currentFactor)); 
        targetMesh->set_transform(t); 

        transformOutput->setData(VariableValue(targetMesh)); 

        for (auto& link : execOutput->getLinks()) { 
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true); 
        }

        setExecutionPending(false); 
    }
}
