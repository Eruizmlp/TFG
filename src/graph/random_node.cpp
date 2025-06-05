#include "random_node.h"
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 

namespace GraphSystem {


    namespace { 
        bool seeded = []() {  
            std::srand(static_cast<unsigned>(std::time(nullptr))); 
            return true; 
            }();
    }

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

    void RandomNode::execute() {
        if (!isExecutionPending()) return; 
        setExecutionPending(false); 
        float minVal = minInput->hasData() ? minInput->getFloat() : defaultMin; 
        float maxVal = maxInput->hasData() ? maxInput->getFloat() : defaultMax;  
        if (minVal > maxVal) std::swap(minVal, maxVal);  

        float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); 
        float value = minVal + r * (maxVal - minVal); 

        resultOutput->setData(VariableValue(value));  
        for (auto& link : execOutput->getLinks()) {  
            if (auto* next = link->getTargetNode())
                next->setExecutionPending(true); 
        }
    }

}
