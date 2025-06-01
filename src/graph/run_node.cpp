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
        if (!isExecutionPending()) {
            std::cout << "[RunNode] Not pending, skipping execution.\n";
            return;
        }

        setExecutionPending(false);  // Reset execution flag

        std::cout << "[RunNode] Executing event: " << eventName << "\n";

        // Access links through execution output
        const auto& links = execOutput->getLinks();

        if (links.empty()) {
            std::cerr << "[RunNode] No linked nodes! Execution stopping here.\n";
            return;
        }

        std::cout << "[RunNode] Found " << links.size() << " links to trigger.\n";

        for (const auto& link : links) {
            if (auto targetNode = link->getTargetNode()) {
                std::cout << "[RunNode] Triggering: " << targetNode->getName() << "\n";
                targetNode->setExecutionPending(true);
            }
            else {
                std::cerr << "[RunNode] Link with null target node!\n";
            }
        }
    }

} // namespace GraphSystem
