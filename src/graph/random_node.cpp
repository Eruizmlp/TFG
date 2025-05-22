#include "random_node.h"
#include <cstdlib>  
#include <ctime>    

namespace GraphSystem {

    RandomNode::RandomNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        minInput = addInput("Min", IOType::FLOAT);
        maxInput = addInput("Max", IOType::FLOAT);

        execOutput = addOutput("Exec", IOType::EXECUTION);
        resultOutput = addOutput("Result", IOType::FLOAT);

        std::srand(static_cast<unsigned>(std::time(nullptr))); 
    }

    void RandomNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        float minVal = minInput->hasData() ? minInput->getFloat() : defaultMin;
        float maxVal = maxInput->hasData() ? maxInput->getFloat() : defaultMax;

        float r = static_cast<float>(std::rand()) / RAND_MAX; 
        float value = minVal + r * (maxVal - minVal);

        resultOutput->setData(value);

        for (auto& link : execOutput->getLinks()) {
            if (auto* next = link->getTargetNode())
                next->setExecutionPending(true);
        }
    }

}
