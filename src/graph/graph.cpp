#include "graph.h"
#include "run_node.h"
#include <iostream>
#include <queue>
#include "tick_node.h"


namespace GraphSystem {

    Graph::Graph(const std::string& name)
        : m_name(name) {
    }

    Graph::~Graph() {
        clear();
    }

    void Graph::addNode(GraphNode* node) {
        if (!node) return;
        for (auto* n : nodes) if (n == node) return;

        nodes.push_back(node);
        std::cout << "[Graph] Added node: " << node->getName() << "\n";

        if (auto* runNode = dynamic_cast<RunNode*>(node)) {
            eventNodes.push_back(runNode);
        }

        if (auto* tickNode = dynamic_cast<TickNode*>(node)) {
            tickNodes.push_back(tickNode);
        }
    }


    void Graph::removeNode(GraphNode* node) {
        if (!node) return;

        // Remove links associated with this node
        for (auto it = links.begin(); it != links.end();) {
            if ((*it)->getSourceNode() == node || (*it)->getTargetNode() == node) {
                delete* it;
                it = links.erase(it);
            }
            else {
                ++it;
            }
        }

        // Remove node
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if (*it == node) {
                delete* it;
                nodes.erase(it);
                break;
            }
        }
    }

    bool Graph::connect(GraphNode* source,
        const std::string& outputName,
        GraphNode* target,
        const std::string& inputName) {
        if (!source || !target) {
            std::cerr << "[Error] Cannot connect - null nodes\n";
            return false;
        }
        if (source == target) {
            std::cerr << "[Error] Cannot connect node to itself\n";
            return false;
        }

        // Find source output
        Output* output = nullptr;
        for (auto* out : source->getOutputs()) {
            if (out && out->getName() == outputName) {
                output = out;
                break;
            }
        }
        if (!output) {
            std::cerr << "[Error] Output '" << outputName << "' not found\n";
            return false;
        }

        // Find target input
        Input* input = nullptr;
        if (!inputName.empty()) {
            for (auto* in : target->getInputs()) {
                if (in && in->getName() == inputName) {
                    input = in;
                    break;
                }
            }
            if (!input) {
                std::cerr << "[Error] Input '" << inputName << "' not found\n";
                return false;
            }
            if (output->getType() != input->getType()) {
                std::cerr << "[Error] Type mismatch between "
                    << outputName << " and " << inputName << "\n";
                return false;
            }
        }
        else {
            // Auto-match by type
            for (auto* in : target->getInputs()) {
                if (!in) continue;
                if (output->getType() == IOType::EXECUTION) {
                    if (in->getName() == "Execute" && in->getType() == IOType::EXECUTION) {
                        input = in;
                        break;
                    }
                }
                else if (in->getType() == output->getType()) {
                    input = in;
                    break;
                }
            }
            if (!input) {
                std::cerr << "[Error] No compatible input found for output '" << outputName << "'\n";
                return false;
            }
        }

        // Check for duplicate link
        for (auto* link : links) {
            if (link->getOutput() == output && link->getTargetNode() == target && link->getTargetInput() == input) {
                std::cerr << "[Warning] Duplicate connection\n";
                return false;
            }
        }

        // Create and store link
        try {
            Link* link = new Link(output, target, input);

            if (output && input) {
                link->transferData();
            }

            links.push_back(link);
            std::cout << "[Graph] Connected " << source->getName() << ":" << outputName
                << " to " << target->getName() << ":" << input->getName() << "\n";
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "[Error] Failed to create link: " << e.what() << "\n";
            return false;
        }
    }


    void Graph::execute() {
        std::cout << "[Graph] Starting execution\n";
        std::queue<GraphNode*> executionQueue;

        for (auto* node : eventNodes) {
            if (node) {
                std::cout << "[Graph] Found event node: " << node->getName() << "\n";
                node->setExecutionPending(true);
                executionQueue.push(node);
            }
        }

        while (!executionQueue.empty()) {
            auto* current = executionQueue.front();
            executionQueue.pop();
            if (!current || !current->isExecutionPending()) {
                continue;
            }

            try {
                current->execute();
                for (auto* out : current->getOutputs()) {
                    if (out->getType() == IOType::EXECUTION) {
                        for (auto* link : out->getLinks()) {
                            if (auto* tgt = link->getTargetNode()) {
                                tgt->setExecutionPending(true);
                                executionQueue.push(tgt);
                            }
                        }
                    }
                }
            }
            catch (...) {
                std::cerr << "[Error] Node execution failed: " << current->getName() << "\n";
            }
            current->setExecutionPending(false);
        }
        std::cout << "[Graph] Execution complete\n";
    }

    void Graph::triggerEventNodes() {
        for (auto* node : eventNodes) {
            if (node) {
                std::cout << "[Graph] Triggering event node: " << node->getName() << "\n";
                node->setExecutionPending(true);
            }
        }
    }

    void Graph::update(float dt) {
        // First tick all TickNodes
        for (auto* tick : tickNodes) {
            if (tick) {
                tick->update(dt);  // TickNode execution
            }
        }

        // Then regular execution
        bool didRun;
        do {
            didRun = false;
            for (auto* node : nodes) {
                if (!node->isExecutionPending())
                    continue;

                node->execute();

                for (auto* out : node->getOutputs()) {
                    if (out->getType() != IOType::EXECUTION)
                        continue;
                    for (auto* link : out->getLinks()) {
                        if (auto* tgt = link->getTargetNode()) {
                            tgt->setExecutionPending(true);
                        }
                    }
                }

                node->setExecutionPending(false);
                didRun = true;
            }
        } while (didRun);
    }

    void Graph::clear() {
        for (auto* link : links) delete link;
        for (auto* node : nodes) delete node;
        links.clear();
        nodes.clear();
        eventNodes.clear();
        tickNodes.clear();
    }

} 
