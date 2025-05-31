#include "link.h"
#include "graph_node.h"
#include <stdexcept>
#include <iostream>

namespace GraphSystem {

    Link::Link(Output* output, GraphNode* target, Input* input)
        : outputRef(output), targetNode(target), targetInput(input)
    {
        // Validate parameters
        if (!output || !target) {
            throw std::invalid_argument("Link cannot be created with null output or target");
        }

        // Verify type compatibility if input is specified
        if (input && output->getType() != input->getType()) {
            throw std::runtime_error("Type mismatch between output and input");
        }

        // Register this link with the output
        output->addLink(this);
    }

    Link::~Link() {
        // Unregister this link from the output
        if (outputRef) {
            outputRef->removeLink(this);
        }
    }

    GraphNode* Link::getSourceNode() const {
        if (!outputRef) return nullptr;

        // This requires Output to have getOwnerNode() implemented
        GraphNode* owner = outputRef->getOwnerNode();
        if (!owner) {
            throw std::logic_error("Output has no owner node");
        }
        return owner;
    }

    void Link::render() {
       
    }

    void Link::transferData() {
        if (!outputRef || !targetNode) {
            throw std::runtime_error("Cannot transfer data - link is invalid");
        }

        if (outputRef->getType() == IOType::EXECUTION) {
            targetNode->setExecutionPending(true);
            return;
        }

        if (!targetInput) {
            throw std::runtime_error("Cannot transfer data - no target input specified");
        }

        targetInput->connectTo(outputRef);
    }

} // namespace GraphSystem
