#include "translate_node.h"
#include <iostream>
#include <queue>

namespace GraphSystem {

    TranslateNode::TranslateNode(const std::string& name, const glm::vec3& off)
        : GraphNode(name, NodeCategory::TRANSFORM), offset(off)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        transformInput = addInput("Transform", IOType::MESH);
        offsetInput = addInput("Offset", IOType::VEC3);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        transformOutput = addOutput("Transform", IOType::MESH);

        offsetInput->setData(VariableValue(this->offset));
        transformOutput->setData(VariableValue(static_cast<MeshInstance3D*>(nullptr)));
    }

    void TranslateNode::setOffset(const glm::vec3& o) {
        offset = o;
    }

    glm::vec3 TranslateNode::getOffset() const {
        if (offsetInput && offsetInput->hasData()) {
            VariableValue v = offsetInput->getValue();
            if (const glm::vec3* pval = std::get_if<glm::vec3>(&v)) {
                return *pval;
            }
        }
        return offset;
    }

    void TranslateNode::execute(std::queue<GraphNode*>& executionQueue) {

        MeshInstance3D* currentMesh = nullptr;
        if (transformInput && transformInput->hasData()) {
            VariableValue v = transformInput->getValue();
            if (auto pval = std::get_if<MeshInstance3D*>(&v)) {
                currentMesh = *pval;
            }
            else {
                std::cerr << "[TranslateNode] (" << getName() << ") Type mismatch en Transform. Se esperaba MESH.\n";
            }
        }

        if (currentMesh) {
            glm::vec3 currentOffset = getOffset();

            Transform t = currentMesh->get_transform();
            t.set_position(currentOffset);
            currentMesh->set_transform(t);

            transformOutput->setData(VariableValue(currentMesh));
        }
        else {
            std::cerr << "[TranslateNode] (" << getName() << ") No hay malla de destino para trasladar.\n";
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
