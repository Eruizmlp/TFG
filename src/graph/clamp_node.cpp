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
        resultOutput->setComputeFunction([this]() -> VariableValue {
            float val = valueInput->hasData() ? valueInput->getFloat() : defaultValue;
            float min_val = minInput->hasData() ? minInput->getFloat() : defaultMin; 
            float max_val = maxInput->hasData() ? maxInput->getFloat() : defaultMax; 
            return VariableValue(std::clamp(val, min_val, max_val)); 
            });
        resultOutput->setData(VariableValue(std::clamp(defaultValue, defaultMin, defaultMax))); 
    }

    void ClampNode::execute()
    {
    }

}
