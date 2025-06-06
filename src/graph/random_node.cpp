#include "random_node.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <queue>

namespace GraphSystem {

    namespace {
        // Esta inicialización de la semilla aleatoria está bien.
        bool seeded = []() {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            return true;
            }();
    }

    // El constructor no necesita cambios.
    RandomNode::RandomNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        minInput = addInput("Min", IOType::FLOAT);
        maxInput = addInput("Max", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setData(VariableValue(0.0f));
    }

    void RandomNode::execute(std::queue<GraphNode*>& executionQueue) {

        float minVal = defaultMin;
        float maxVal = defaultMax;

        if (minInput && minInput->hasData()) {
            VariableValue v = minInput->getValue();
            if (const float* pval = std::get_if<float>(&v)) minVal = *pval;
            else std::cerr << "[RandomNode] (" << getName() << ") Type mismatch for Min input.\n";
        }
        if (maxInput && maxInput->hasData()) {
            VariableValue v = maxInput->getValue();
            if (const float* pval = std::get_if<float>(&v)) maxVal = *pval;
            else std::cerr << "[RandomNode] (" << getName() << ") Type mismatch for Max input.\n";
        }

        if (minVal > maxVal) std::swap(minVal, maxVal);

        float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float value = minVal + r * (maxVal - minVal);

        resultOutput->setData(VariableValue(value));

        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }
}
