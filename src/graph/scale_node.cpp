#include "scale_node.h"
#include <iostream>
#include <glm/common.hpp>
#include <cmath>
#include <queue>
#include <fstream>

namespace GraphSystem {

    ScaleNode::ScaleNode(const std::string& name, float fact)
        : GraphNode(name, NodeCategory::TRANSFORM), factor(fact)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        transformInput = addInput("Transform", IOType::MESH);
        factorInput = addInput("Factor", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        transformOutput = addOutput("Transform", IOType::MESH);

        factorInput->setData(this->factor);
      
        transformOutput->setData(static_cast<MeshInstance3D*>(nullptr));
    }

    void ScaleNode::setScaleFactor(float f) {
        factor = f;
    }

  
    float ScaleNode::getScaleFactor() const {
      
        if (factorInput) {
            return factorInput->getFloat();
        }
        return factor;
    }


    void ScaleNode::execute(std::queue<GraphNode*>& executionQueue) {
        
        MeshInstance3D* currentMesh = transformInput ? transformInput->getMesh() : nullptr;

        if (currentMesh) {
            float currentFactor = getScaleFactor();

            currentMesh->set_scale(glm::vec3(currentFactor));
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

    void ScaleNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
  
        file.write(reinterpret_cast<const char*>(&factor), sizeof(factor));
    }

    void ScaleNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
  
        file.read(reinterpret_cast<char*>(&factor), sizeof(factor));
    }

    void ScaleNode::rebindPins() {
       
        execInput = getInput("Execute");
        transformInput = getInput("Transform");
        factorInput = getInput("Factor");
        execOutput = getOutput("Exec");
        transformOutput = getOutput("Transform");

        if (factorInput) {
            factorInput->setData(this->factor);
        }
    }
}
