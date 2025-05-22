#include "tick_node.h"
using namespace GraphSystem;

TickNode::TickNode(const std::string& name)
    : GraphNode(name, NodeCategory::FLOW)
{
    // Two Exec inputs: Start and Stop
    startInput = addInput("Start", IOType::EXECUTION);
    stopInput = addInput("Stop", IOType::EXECUTION);

    // One Exec output: Tick
    tickOutput = addOutput("Tick", IOType::EXECUTION);
}

void TickNode::execute() {

    if (!isExecutionPending()) return;
    setExecutionPending(false);

    running = !running;
}

void TickNode::update(float dt) {

    if (!running) return;

    for (auto* link : tickOutput->getLinks()) {
        if (auto* tgt = link->getTargetNode())
            tgt->setExecutionPending(true);
    }
}
