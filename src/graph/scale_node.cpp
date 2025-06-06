#include "scale_node.h"
#include <iostream>
#include <glm/common.hpp>
#include <cmath>
#include <queue>

namespace GraphSystem {

    // El constructor y los helpers que tenías están perfectos.
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

    void ScaleNode::setScaleFactor(float f) {
        factor = f;
    }

    float ScaleNode::getScaleFactor() const {
        if (factorInput && factorInput->hasData()) {
            VariableValue v = factorInput->getValue();
            if (const float* pval = std::get_if<float>(&v)) {
                return *pval;
            }
        }
        return factor;
    }

    // --- MÉTODO EXECUTE CORREGIDO ---
    void ScaleNode::execute(std::queue<GraphNode*>& executionQueue) {

        MeshInstance3D* currentMesh = nullptr;
        if (transformInput && transformInput->hasData()) {
            VariableValue v = transformInput->getValue();
            if (auto pval = std::get_if<MeshInstance3D*>(&v)) {
                currentMesh = *pval;
            }
            else {
                std::cerr << "[ScaleNode] (" << getName() << ") Type mismatch en Transform. Se esperaba MESH.\n";
            }
        }

        // 2. Realizar la acción de escalar si es posible.
        if (currentMesh) {
            float currentFactor = getScaleFactor();
            currentFactor = glm::clamp(std::abs(currentFactor), 0.01f, 100.0f);

            Transform t = currentMesh->get_transform();
            t.set_scale(glm::vec3(currentFactor));
            currentMesh->set_transform(t);

            transformOutput->setData(VariableValue(currentMesh));
        }
        else {
            std::cerr << "[ScaleNode] (" << getName() << ") No hay malla de destino para escalar.\n";
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
