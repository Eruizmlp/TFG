#include "random_node.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <queue>
#include <fstream>

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

       
        minInput->setData(defaultMin);
        maxInput->setData(defaultMax);
        
        resultOutput->setData(0.0f);
    }


    void RandomNode::execute(std::queue<GraphNode*>& executionQueue) {

        float minVal = minInput->getFloat();
        float maxVal = maxInput->getFloat();

        if (minVal > maxVal) std::swap(minVal, maxVal);

        float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float value = minVal + r * (maxVal - minVal);

        resultOutput->setData(value);


        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }


    void RandomNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
        
        file.write(reinterpret_cast<const char*>(&defaultMin), sizeof(defaultMin));
        file.write(reinterpret_cast<const char*>(&defaultMax), sizeof(defaultMax));
    }

    void RandomNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
        
        file.read(reinterpret_cast<char*>(&defaultMin), sizeof(defaultMin));
        file.read(reinterpret_cast<char*>(&defaultMax), sizeof(defaultMax));
    }

    void RandomNode::rebindPins() {
       
        execInput = getInput("Execute");
        minInput = getInput("Min");
        maxInput = getInput("Max");
        execOutput = getOutput("Exec");
        resultOutput = getOutput("Result");

      
        if (minInput) minInput->setData(defaultMin);
        if (maxInput) maxInput->setData(defaultMax);
    }
}
