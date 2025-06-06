#include "clamp_node.h"
#include <algorithm>
#include <iostream>

namespace GraphSystem {

    ClampNode::ClampNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        valueInput = addInput("Value", IOType::FLOAT);
        minInput = addInput("Min", IOType::FLOAT);
        maxInput = addInput("Max", IOType::FLOAT);

        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> VariableValue {
            float val = defaultMin;
            float min_val = defaultMin;
            float max_val = defaultMax;

            if (valueInput->hasData()) {
                VariableValue v = valueInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) val = *pval;
                else std::cerr << "[ClampNode] (" << getName() << ") Type mismatch for Value input. Expected FLOAT.\n";
            }
            if (minInput->hasData()) {
                VariableValue v = minInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) min_val = *pval;
                else std::cerr << "[ClampNode] (" << getName() << ") Type mismatch for Min input. Expected FLOAT.\n";
            }
            if (maxInput->hasData()) {
                VariableValue v = maxInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) max_val = *pval;
                else std::cerr << "[ClampNode] (" << getName() << ") Type mismatch for Max input. Expected FLOAT.\n";
            }

            return VariableValue(std::clamp(val, min_val, max_val));
            });

        resultOutput->setData(VariableValue(std::clamp(defaultValue, defaultMin, defaultMax)));
    }

    void ClampNode::execute(std::queue<GraphNode*>& executionQueue) {

    }
}
