#include "run_node.h"
#include <iostream>
#include <queue>
#include <fstream>

namespace GraphSystem {

    RunNode::RunNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW),
        eventName("DefaultEvent")
    {
        execOutput = addOutput("Execution", IOType::EXECUTION);
        setEntryPoint(true);
    }

    RunNode::~RunNode() {
        
    }

    void RunNode::execute(std::queue<GraphNode*>& executionQueue) {
        if (execOutput) {
            for (auto* link : execOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }


    void RunNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);

        uint64_t size = eventName.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(eventName.c_str(), size);
    }

    void RunNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
        
        uint64_t size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        eventName.resize(size);
        if (size > 0) {
            file.read(&eventName[0], size);
        }
    }

    void RunNode::rebindPins() {
       
        execOutput = getOutput("Execution");
    }
}
