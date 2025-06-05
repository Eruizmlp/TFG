#include "loop_node.h"

namespace GraphSystem {

    LoopNode::LoopNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);  
        countInput = addInput("Count", IOType::FLOAT); 

        loopExecOutput = addOutput("LoopExec", IOType::EXECUTION);  
        completedOutput = addOutput("Completed", IOType::EXECUTION);  
        indexOutput = addOutput("Index", IOType::FLOAT); 
    
        indexOutput->setData(VariableValue(0.0f));
    }

    void LoopNode::execute() {
        if (!isExecutionPending()) return; 
        setExecutionPending(false); 
        float countVal = countInput->hasData() ? countInput->getFloat() : static_cast<float>(repeatCount);
        repeatCount = static_cast<int>(countVal);

        currentIndex = 0; 
        looping = (repeatCount > 0); 
        if (!looping) { 
            for (auto& link : completedOutput->getLinks()) {
                if (auto* next = link->getTargetNode()) {
                    next->setExecutionPending(true); 
                }
            }
        }
        else {
            indexOutput->setData(VariableValue(static_cast<float>(currentIndex))); 
        }
    }

    void LoopNode::update(float deltaTime) { 

        if (!looping) return; 

     

        if (currentIndex < repeatCount) {
            indexOutput->setData(VariableValue(static_cast<float>(currentIndex)));  
            for (auto& link : loopExecOutput->getLinks()) { 
                if (auto* next = link->getTargetNode()) {
                    next->setExecutionPending(true); 
                }
            }


            ++currentIndex; 
            if (currentIndex >= repeatCount) { 
                looping = false; 
                for (auto& link : completedOutput->getLinks()) {  
                    if (auto* next = link->getTargetNode()) {
                        next->setExecutionPending(true); 
                    }
                }
            }
        }

    }
}
