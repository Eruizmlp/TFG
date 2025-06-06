#include "tick_node.h"
#include <queue>

namespace GraphSystem {

    TickNode::TickNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        startInput = addInput("Start", IOType::EXECUTION);
        stopInput = addInput("Stop", IOType::EXECUTION);
        tickOutput = addOutput("Tick", IOType::EXECUTION);
    }

    void TickNode::execute(std::queue<GraphNode*>& executionQueue) {
        
        running = !running;
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
}
