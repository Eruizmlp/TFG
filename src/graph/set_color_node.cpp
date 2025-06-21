#include "set_color_node.h"
#include "framework/nodes/mesh_instance_3d.h"
#include "graphics/material.h"
#include <iostream>

namespace GraphSystem {

    SetColorNode::SetColorNode(const std::string& name)
        : GraphNode(name, NodeCategory::TRANSFORM)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        transformInput = addInput("Transform", IOType::MESH);
   
        rInput = addInput("R", IOType::FLOAT);
        gInput = addInput("G", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        aInput = addInput("A", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        transformOutput = addOutput("Transform", IOType::MESH);

        rInput->setData(defaultR);
        gInput->setData(defaultG);
        bInput->setData(defaultB);
        aInput->setData(defaultA);
    }

    void SetColorNode::execute(std::queue<GraphNode*>& executionQueue) {
        MeshInstance3D* currentMesh = transformInput ? transformInput->getMesh() : nullptr;

        if (currentMesh) {
       
            float r = rInput->getFloat();
            float g = gInput->getFloat();
            float b = bInput->getFloat();
            float a = aInput->getFloat();
            glm::vec4 newColor(r, g, b, a);

            Material* mat = currentMesh->get_surface_material(0);
            if (mat) {
                mat->set_color(newColor);
            }

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


    void SetColorNode::setDefaultR(float r) { defaultR = r; if (rInput) rInput->setData(r); }
    void SetColorNode::setDefaultG(float g) { defaultG = g; if (gInput) gInput->setData(g); }
    void SetColorNode::setDefaultB(float b) { defaultB = b; if (bInput) bInput->setData(b); }
    void SetColorNode::setDefaultA(float a) { defaultA = a; if (aInput) aInput->setData(a); }

    void SetColorNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        file.write(reinterpret_cast<const char*>(&defaultR), sizeof(defaultR));
        file.write(reinterpret_cast<const char*>(&defaultG), sizeof(defaultG));
        file.write(reinterpret_cast<const char*>(&defaultB), sizeof(defaultB));
        file.write(reinterpret_cast<const char*>(&defaultA), sizeof(defaultA));
    }

    void SetColorNode::parse(std::ifstream& file) {
        GraphNode::parse(file);

        file.read(reinterpret_cast<char*>(&defaultR), sizeof(defaultR));
        file.read(reinterpret_cast<char*>(&defaultG), sizeof(defaultG));
        file.read(reinterpret_cast<char*>(&defaultB), sizeof(defaultB));
        file.read(reinterpret_cast<char*>(&defaultA), sizeof(defaultA));
    }

    void SetColorNode::rebindPins() {

        execInput = getInput("Execute");
        transformInput = getInput("Transform");
        rInput = getInput("R");
        gInput = getInput("G");
        bInput = getInput("B");
        aInput = getInput("A");
        execOutput = getOutput("Exec");
        transformOutput = getOutput("Transform");

     
        if (rInput) rInput->setData(defaultR);
        if (gInput) gInput->setData(defaultG);
        if (bInput) bInput->setData(defaultB);
        if (aInput) aInput->setData(defaultA);
    }
}
