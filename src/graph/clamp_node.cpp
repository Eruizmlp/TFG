#include "clamp_node.h"
#include <algorithm>

namespace GraphSystem {

    ClampNode::ClampNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        valueInput = addInput("Value", IOType::FLOAT);
        minInput = addInput("Min", IOType::FLOAT);
        maxInput = addInput("Max", IOType::FLOAT);

        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> float {
            float val = valueInput->hasData() ? valueInput->getFloat() : defaultValue;
            float min = minInput->hasData() ? minInput->getFloat() : defaultMin;
            float max = maxInput->hasData() ? maxInput->getFloat() : defaultMax;
            return std::clamp(val, min, max);
            });

        resultOutput->setData(std::clamp(defaultValue, defaultMin, defaultMax));
    }

}
