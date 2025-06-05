#include "lerp_node.h"

namespace GraphSystem {

    LerpNode::LerpNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        aInput = addInput("A", IOType::FLOAT);  
        bInput = addInput("B", IOType::FLOAT); 
        tInput = addInput("T", IOType::FLOAT);  
        resultOutput = addOutput("Result", IOType::FLOAT); 
        resultOutput->setComputeFunction([this]() -> VariableValue { 
            float a = aInput->hasData() ? aInput->getFloat() : defaultA; 
            float b = bInput->hasData() ? bInput->getFloat() : defaultB; 
            float t = tInput->hasData() ? tInput->getFloat() : defaultT;

            return VariableValue(glm::mix(a, b, std::clamp(t, 0.0f, 1.0f)));  
            });
        resultOutput->setData(VariableValue(glm::mix(defaultA, defaultB, std::clamp(defaultT, 0.0f, 1.0f))));  
    }

    void LerpNode::execute()
    {
    }

}
