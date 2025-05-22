#include "run_node.h"
#include <iostream>

namespace GraphSystem {

    RunNode::RunNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW),
        eventName("DefaultEvent"),
        execOutput(addOutput("Execution", IOType::EXECUTION))  
    {
        setEntryPoint(true);
        setExecutionPending(false);  
    }

    RunNode::~RunNode() {
        std::cout << "[EventNode] Destroying event: " << eventName << "\n";
    }

    void RunNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);  // Reset execution flag

        std::cout << "[EventNode] Executing event: " << eventName << "\n";

        // Access links through execution output
        const auto& links = execOutput->getLinks();

        if (links.empty()) {
            std::cerr << "[EventNode] No linked nodes! Execution stopping here.\n";
            return;
        }

        for (const auto& link : links) {
            if (auto targetNode = link->getTargetNode()) {
                std::cout << "[EventNode] Triggering: " << targetNode->getName() << "\n";
                targetNode->setExecutionPending(true);
            }
        }
    }

} // namespace GraphSystem
