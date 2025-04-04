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
        // Validate link state
        if (!outputRef || !targetNode) {
            throw std::runtime_error("Cannot transfer data - link is invalid");
        }

        // Handle execution flow
        if (outputRef->getType() == IOType::EXECUTION) {
            targetNode->setExecutionPending(true);
            return;
        }

        // Handle data transfer
        if (!targetInput) {
            throw std::runtime_error("Cannot transfer data - no target input specified");
        }

        if (!outputRef->hasData()) {
            throw std::runtime_error("Cannot transfer data - output has no data");
        }

        // Type-safe data transfer
        try {
            switch (outputRef->getType()) {
            case IOType::BOOL:
                targetInput->setData(outputRef->getBool());
                break;
            case IOType::INT:
                targetInput->setData(outputRef->getInt());
                break;
            case IOType::FLOAT:
                targetInput->setData(outputRef->getFloat());
                break;
            case IOType::STRING:
                targetInput->setData(outputRef->getString());
                break;
            case IOType::VEC2:
                targetInput->setData(outputRef->getVec2());
                break;
            case IOType::VEC3:
                targetInput->setData(outputRef->getVec3());
                break;
            case IOType::VEC4:
                targetInput->setData(outputRef->getVec4());
                break;
            case IOType::MAT4:
                targetInput->setData(outputRef->getMat4());
                break;
            case IOType::TRANSFORM:
                targetInput->setData(outputRef->getTransform());
                break;
            case IOType::EXECUTION:
                // Handled earlier
                break;
            default:
                throw std::runtime_error("Unsupported data type for transfer");
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error(std::string("Data transfer failed: ") + e.what());
        }
    }

} // namespace GraphSystem
