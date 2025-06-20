#include "tick_node.h"
#include <queue>


#include <sstream>
#include <fstream>

namespace GraphSystem {

    TickNode::TickNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        startInput = addInput("Start", IOType::EXECUTION);
        stopInput = addInput("Stop", IOType::EXECUTION);
        tickOutput = addOutput("Tick", IOType::EXECUTION);
    }

    void TickNode::execute(std::queue<GraphNode*>& executionQueue) {        
        if (tickOutput) {
            for (auto* link : tickOutput->getLinks()) {
                if (auto* nextNode = link->getTargetNode()) {
                    executionQueue.push(nextNode);
                }
            }
        }
    }

    void TickNode::update(float delta_time) {
    }

    void TickNode::start() {
        running = true;
    }

    void TickNode::stop() {
        running = false;
    }

    bool TickNode::isRunning() const {
        return running;
    }

    void TickNode::rebindPins() {
        startInput = getInput("Start");
        stopInput = getInput("Stop");
        tickOutput = getOutput("Tick");
    }

    void TickNode::serialize(std::ofstream& file)
    {
        GraphNode::serialize(file);
        file.write(reinterpret_cast<const char*>(&running), sizeof(running));

    }
    void TickNode::parse(std::ifstream& file)
    {
        GraphNode::parse(file);
        file.read(reinterpret_cast<char*>(&running), sizeof(running));
    }

}
