#include "graph.h"
#include "run_node.h"
#include <iostream>

namespace GraphSystem {

    Graph::Graph(const std::string& name)
        : m_name(name)
    {
    }

    Graph::~Graph() {
        clear();
    }

    void Graph::addNode(GraphNode* node) {
        if (!node) return;
        for (auto* n : nodes) if (n == node) return;
        nodes.push_back(node);
        std::cout << "[Graph] Added node: " << node->getName() << "\n";
    }

    void Graph::removeNode(GraphNode* node) {
        if (!node) return;
        for (auto it = links.begin(); it != links.end();) {
            if ((*it)->getSourceNode() == node ||
                (*it)->getTargetNode() == node) {
                delete* it;
                it = links.erase(it);
            }
            else ++it;
        }
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
        const std::string& inputName)
    {
        if (!source || !target) {
            std::cerr << "[Error] Cannot connect - null nodes\n";
            return false;
        }
        if (source == target) {
            std::cerr << "[Error] Cannot connect node to itself\n";
            return false;
        }

        // find source output
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

        // find target input (if given)
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

        // check duplicates
        for (auto* link : links) {
            if (link->getOutput() == output &&
                link->getTargetNode() == target &&
                link->getTargetInput() == input) {
                std::cerr << "[Warning] Duplicate connection\n";
                return false;
            }
        }

        // create it
        try {
            Link* link = new Link(output, target, input);
            links.push_back(link);
            std::cout << "[Graph] Connected "
                << source->getName() << ":" << outputName
                << " to " << target->getName() << "\n";
            return true;
        }
        catch (...) {
            std::cerr << "[Error] Failed to create link\n";
            return false;
        }
    }

    void Graph::render() {
        for (auto* node : nodes) node->render();
        for (auto* link : links) link->render();
    }

    // one-shot execution (unchanged)
    void Graph::execute() {
        std::cout << "[Graph] Starting execution\n";
        std::queue<GraphNode*> executionQueue;

        // enqueue entry-points
        for (auto* node : nodes) {
            if (node && node->isEntryPoint()) {
                std::cout << "[Graph] Found entry point: " << node->getName() << "\n";
                node->setExecutionPending(true);
                executionQueue.push(node);
            }
        }

        // drain
        while (!executionQueue.empty()) {
            auto* current = executionQueue.front();
            executionQueue.pop();
            if (!current || !current->isExecutionPending()) {
                std::cout << "[Graph] Skipping node (not pending)\n";
                continue;
            }
            std::cout << "[Graph] Executing: " << current->getName() << "\n";
            try {
                current->execute();
                // enqueue downstream Exec ports
                for (auto* out : current->getOutputs()) {
                    if (out->getType() == IOType::EXECUTION) {
                        for (auto* link : out->getLinks()) {
                            if (auto* tgt = link->getTargetNode()) {
                                std::cout << "[Graph] Adding to queue: " << tgt->getName() << "\n";
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

    // Trigger entry-points without executing 
    void Graph::triggerEntryPoints() {
        for (auto* node : nodes) {
            if (node->isEntryPoint()) {
                std::cout << "[Graph] Triggering entry point: " << node->getName() << "\n";
                node->setExecutionPending(true);
            }
        }
    }



    void Graph::update(float dt) {
        
        for (auto* node : nodes) {
            node->update(dt);
        }

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
    }

} 
