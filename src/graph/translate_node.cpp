#include "translate_node.h"

namespace GraphSystem {

    TranslateNode::TranslateNode(const std::string& name,
        const glm::vec3& off) 
        : GraphNode(name, NodeCategory::TRANSFORM), 
        offset(off)  
    {
        execInput = addInput("Execute", IOType::EXECUTION);  
        transformInput = addInput("Transform", IOType::MESH);  
        offsetInput = addInput("Offset", IOType::VEC3); 

        execOutput = addOutput("Exec", IOType::EXECUTION);  
        transformOutput = addOutput("Transform", IOType::MESH); 

        offsetInput->setData(VariableValue(this->offset)); 
        transformOutput->setData(VariableValue(static_cast<MeshInstance3D*>(nullptr))); 
    }

    void TranslateNode::setTarget(MeshInstance3D* mesh) {
        targetMesh = mesh; 
    }

    void TranslateNode::setOffset(const glm::vec3& o) {
        offset = o; 
    }

    glm::vec3 TranslateNode::getOffset() const { 
        if (offsetInput && offsetInput->getConnectedOutput() && offsetInput->getConnectedOutput()->hasData()) {
            return offsetInput->getVec3();
        }
        return offset;
    }

    void TranslateNode::execute() {
        if (!isExecutionPending()) return; 

        if (!targetMesh) { 
            if (transformInput && transformInput->getConnectedOutput() && transformInput->getConnectedOutput()->hasData()) { 
                targetMesh = transformInput->getConnectedOutput()->getMesh(); 
            }
        }

        if (!targetMesh) { 
            setExecutionPending(false);
            return;
        }

        glm::vec3 currentOffset = offset; 
        if (offsetInput && offsetInput->getConnectedOutput() && offsetInput->getConnectedOutput()->hasData()) { 
            currentOffset = offsetInput->getConnectedOutput()->getVec3(); 
        }

        Transform t = targetMesh->get_transform();

        t.set_position(currentOffset); 
        targetMesh->set_transform(t);  

        transformOutput->setData(VariableValue(targetMesh)); 

        for (auto& link : execOutput->getLinks()) { 
            if (auto next = link->getTargetNode())
                next->setExecutionPending(true); 
        }

        setExecutionPending(false); 
    }
}
