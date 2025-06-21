#include "translate_node.h"
#include <iostream>
#include <queue>
#include <fstream>

namespace GraphSystem {

  
    TranslateNode::TranslateNode(const std::string& name, const glm::vec3& off)
        : GraphNode(name, NodeCategory::TRANSFORM), offset(off)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        transformInput = addInput("Transform", IOType::MESH);
        offsetInput = addInput("Offset", IOType::VEC3);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        transformOutput = addOutput("Transform", IOType::MESH);

       
        offsetInput->setData(this->offset);
        transformOutput->setData(static_cast<MeshInstance3D*>(nullptr));
    }

    void TranslateNode::setOffset(const glm::vec3& o) {
        offset = o;
       
        if (offsetInput) {
            offsetInput->setData(offset);
        }
    }

    glm::vec3 TranslateNode::getOffset() const {
    
        if (offsetInput) {
            return offsetInput->getVec3();
        }
        return offset;
    }


    void TranslateNode::execute(std::queue<GraphNode*>& executionQueue) {
     
        MeshInstance3D* currentMesh = transformInput ? transformInput->getMesh() : nullptr;

        if (currentMesh) {

            glm::vec3 currentOffset = getOffset();
            Transform t = currentMesh->get_transform();
            t.set_position(currentOffset);
            currentMesh->set_transform(t);

           
            transformOutput->setData(currentMesh);
        }
        else {
            transformOutput->setData(static_cast<MeshInstance3D*>(nullptr));
        }

        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }



    void TranslateNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        file.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
    }

    void TranslateNode::parse(std::ifstream& file) {
        GraphNode::parse(file);

        file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
    }

    void TranslateNode::rebindPins() {

        execInput = getInput("Execute");
        transformInput = getInput("Transform");
        offsetInput = getInput("Offset");
        execOutput = getOutput("Exec");
        transformOutput = getOutput("Transform");

        if (offsetInput) {
            offsetInput->setData(this->offset);
        }
    }
}
