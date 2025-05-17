#include "clamp_node.h"
#include <algorithm>

namespace GraphSystem {

    ClampNode::ClampNode(const std::string& name)
        : GraphNode(name)
    {
        valueInput = addInput("Value", IOType::FLOAT);
        minInput = addInput("Min", IOType::FLOAT);
        maxInput = addInput("Max", IOType::FLOAT);

        resultOutput = addOutput("Result", IOType::FLOAT);
    }

    void ClampNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float val = valueInput->hasData() ? valueInput->getFloat() : defaultValue;
        float min = minInput->hasData() ? minInput->getFloat() : defaultMin;
        float max = maxInput->hasData() ? maxInput->getFloat() : defaultMax;

        float clamped = std::clamp(val, min, max);
        resultOutput->setData(clamped);
    }

}
