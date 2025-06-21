#include "loop_node.h"
#include <iostream>
#include <queue>
#include <fstream>

namespace GraphSystem {


    LoopNode::LoopNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        execInput = addInput("Execute", IOType::EXECUTION);
        countInput = addInput("Count", IOType::FLOAT); 

        loopExecOutput = addOutput("LoopExec", IOType::EXECUTION);
        completedOutput = addOutput("Completed", IOType::EXECUTION);
        indexOutput = addOutput("Index", IOType::FLOAT);

       
        countInput->setData(1.0f); 
        indexOutput->setData(0.0f);
    }

  
    void LoopNode::execute(std::queue<GraphNode*>& executionQueue) {
       
        int loopCount = static_cast<int>(countInput->getFloat());
        if (loopCount < 0) loopCount = 0; 

        
        if (loopCount > 0) {
            for (int i = 0; i < loopCount; ++i) {
               
                indexOutput->setData(static_cast<float>(i));

                
                for (auto* link : loopExecOutput->getLinks()) {
                    if (auto* nextNode = link->getTargetNode()) {
                        executionQueue.push(nextNode);
                    }
                }
            }
        }
       
        for (auto* link : completedOutput->getLinks()) {
            if (auto* nextNode = link->getTargetNode()) {
                executionQueue.push(nextNode);
            }
        }
    }

    void LoopNode::serialize(std::ofstream& file) {
     
        GraphNode::serialize(file);
    }

    void LoopNode::parse(std::ifstream& file) {
       
        GraphNode::parse(file);
    }

    void LoopNode::rebindPins() {
      
        execInput = getInput("Execute");
        countInput = getInput("Count");
        loopExecOutput = getOutput("LoopExec");
        completedOutput = getOutput("Completed");
        indexOutput = getOutput("Index");
    }

  
    void LoopNode::update(float deltaTime) {}
}
