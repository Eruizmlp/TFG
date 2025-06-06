#include "lerp_node.h"
#include <iostream>

namespace GraphSystem {

    LerpNode::LerpNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        tInput = addInput("T", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> VariableValue {
            float a = defaultA;
            float b = defaultB;
            float t = defaultT;

            if (aInput->hasData()) {
                VariableValue v = aInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) a = *pval;
                else std::cerr << "[LerpNode] (" << getName() << ") Type mismatch for input A. Expected FLOAT.\n";
            }
            if (bInput->hasData()) {
                VariableValue v = bInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) b = *pval;
                else std::cerr << "[LerpNode] (" << getName() << ") Type mismatch for input B. Expected FLOAT.\n";
            }
            if (tInput->hasData()) {
                VariableValue v = tInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) t = *pval;
                else std::cerr << "[LerpNode] (" << getName() << ") Type mismatch for input T. Expected FLOAT.\n";
            }

            return VariableValue(glm::mix(a, b, std::clamp(t, 0.0f, 1.0f)));
            });

        resultOutput->setData(VariableValue(glm::mix(defaultA, defaultB, std::clamp(defaultT, 0.0f, 1.0f))));
    }

    void LerpNode::execute(std::queue<GraphNode*>& executionQueue)
    {
    }


}
