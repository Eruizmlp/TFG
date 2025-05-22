#include "lerp_node.h"

namespace GraphSystem {

    LerpNode::LerpNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        tInput = addInput("T", IOType::FLOAT);

        resultOutput = addOutput("Result", IOType::FLOAT);
    }

    void LerpNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float a = aInput->hasData() ? aInput->getFloat() : defaultA;
        float b = bInput->hasData() ? bInput->getFloat() : defaultB;
        float t = tInput->hasData() ? tInput->getFloat() : defaultT;

        float result = a + (b - a) * t;

        resultOutput->setData(result);
    }

}
