// print_node.cpp
#include "print_node.h"
#include <iostream>

namespace GraphSystem {

    PrintNode::PrintNode(const std::string& name) : GraphNode(name) {
        addInput("Execute", IOType::EXECUTION);
        addInput("Message", IOType::STRING);
        std::cout << "[PrintNode] Created: " << name << "\n";
    }

    void PrintNode::execute() {
        if (!isExecutionPending()) return;
        setExecutionPending(false);

        if (Input* messageInput = getInput("Message")) {
            if (messageInput->hasData()) {
                std::cout << getName() << ": " << messageInput->getString() << "\n";
            }
        }
    }

} // namespace GraphSystem
