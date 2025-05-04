// tick_node.cpp
#include "tick_node.h"
using namespace GraphSystem;

TickNode::TickNode(const std::string& name)
    : GraphNode(name)
{
    // Two Exec inputs: Start and Stop
    startInput = addInput("Start", IOType::EXECUTION);
    stopInput = addInput("Stop", IOType::EXECUTION);

    // One Exec output: Tick
    tickOutput = addOutput("Tick", IOType::EXECUTION);
}

void TickNode::execute() {
    // Only run once per Exec pulse
    if (!isExecutionPending()) return;
    setExecutionPending(false);

    // Toggle the running flag on any Exec pulse
    running = !running;
}

void TickNode::update(float dt) {
    // If we're running, emit a Tick every frame
    if (!running) return;

    for (auto* link : tickOutput->getLinks()) {
        if (auto* tgt = link->getTargetNode())
            tgt->setExecutionPending(true);
    }
}
