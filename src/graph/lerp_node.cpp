#include "lerp_node.h"

namespace GraphSystem {

    LerpNode::LerpNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        tInput = addInput("T", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

        resultOutput->setComputeFunction([this]() -> float {
            float a = aInput->hasData() ? aInput->getFloat() : 0.0f;
            float b = bInput->hasData() ? bInput->getFloat() : 1.0f;
            float t = tInput->hasData() ? tInput->getFloat() : 0.5f;

            return glm::mix(a, b, glm::clamp(t, 0.0f, 1.0f));
            });

        resultOutput->setData(0.0f);  
    }

    void LerpNode::execute()
    {
    }

}
