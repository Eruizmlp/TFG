#include "graph.h"
#include "event_node.h"
#include <iostream>

namespace GraphSystem {

    Graph::Graph(const std::string& name, bool isTickGraph)
        : m_name(name), m_isTickGraph(isTickGraph) {}

    Graph::~Graph() {
        clear();
    }

    void Graph::addNode(GraphNode* node) {
        if (!node) return;

        bool exists = false;
        for (auto n : nodes) {
            if (n == node) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            nodes.push_back(node);
            std::cout << "[Graph] Added node: " << node->getName() << "\n";
        }
    }

    void Graph::removeNode(GraphNode* node) {
        if (!node) return;

        // Remove links manually
        auto it = links.begin();
        while (it != links.end()) {
            if ((*it)->getSourceNode() == node || (*it)->getTargetNode() == node) {
                delete* it;
                it = links.erase(it);
            }
            else {
                ++it;
            }
        }

        // Remove node manually
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if (*it == node) {
                delete* it;
                nodes.erase(it);
                break;
            }
        }
    }

    bool Graph::connect(GraphNode* source, const std::string& outputName,
        GraphNode* target, const std::string& inputName) {
        if (!source || !target) {
            std::cerr << "[Error] Cannot connect - null nodes\n";
            return false;
        }

        if (source == target) {
            std::cerr << "[Error] Cannot connect node to itself\n";
            return false;
        }

        // Find source output manually
        Output* output = nullptr;
        for (auto out : source->getOutputs()) {
            if (out && out->getName() == outputName) {
                output = out;
                break;
            }
        }

        if (!output) {
            std::cerr << "[Error] Output '" << outputName << "' not found\n";
            return false;
        }

        // Find target input manually if specified
        Input* input = nullptr;
        if (!inputName.empty()) {
            for (auto in : target->getInputs()) {
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

        // Check for duplicates manually
        bool duplicate = false;
        for (auto link : links) {
            if (link->getOutput() == output &&
                link->getTargetNode() == target &&
                link->getTargetInput() == input) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            std::cerr << "[Warning] Duplicate connection\n";
            return false;
        }

        // Create link
        try {
            Link* link = new Link(output, target, input);
            links.push_back(link);
            std::cout << "[Graph] Connected " << source->getName() << ":" << outputName
                << " to " << target->getName() << "\n";
            return true;
        }
        catch (...) {
            std::cerr << "[Error] Failed to create link\n";
            return false;
        }
    }

    void Graph::render() {
        for (auto node : nodes) node->render();
        for (auto link : links) link->render();
    }

    // graph.cpp
    void Graph::execute() {
        std::cout << "[Graph] Starting execution\n";
        std::queue<GraphNode*> executionQueue;

        // Find entry points
        for (auto node : nodes) {
            if (node && node->isEntryPoint()) {
                std::cout << "[Graph] Found entry point: " << node->getName() << "\n";
                node->setExecutionPending(true);
                executionQueue.push(node);
            }
        }

        // Execute graph
        while (!executionQueue.empty()) {
            auto current = executionQueue.front();
            executionQueue.pop();

            if (!current || !current->isExecutionPending()) {
                std::cout << "[Graph] Skipping node (null or not pending)\n";
                continue;
            }

            std::cout << "[Graph] Executing: " << current->getName() << "\n";
            try {
                current->execute();

                // After execution, collect newly triggered nodes
                for (auto* output : current->getOutputs()) {
                    if (output->getType() == IOType::EXECUTION) {
                        for (auto* link : output->getLinks()) {
                            if (auto target = link->getTargetNode()) {
                                std::cout << "[Graph] Adding to queue: " << target->getName() << "\n";
                                target->setExecutionPending(true);
                                executionQueue.push(target);
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

    void Graph::clear() {
        for (auto link : links) delete link;
        for (auto node : nodes) delete node;
        links.clear();
        nodes.clear();
    }
}
