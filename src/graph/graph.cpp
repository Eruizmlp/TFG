#include "graph.h"
#include "run_node.h"
#include "tick_node.h"
#include "timer_node.h"
#include <iostream>
#include <queue>
#include "mapper_node.h"

namespace GraphSystem {

    Graph::Graph(const std::string& name) : m_name(name) {}

    Graph::~Graph() {
        clear();
    }

    GraphNode* Graph::getNodeByName(const std::string& name) {
        for (auto* node : nodes) {
            if (node->getName() == name) {
                return node;
            }
        }
        return nullptr;
    }


    void Graph::addNode(GraphNode* node) {
        if (!node) return;
        for (auto* n : nodes) if (n == node) return;
        nodes.push_back(node);

        if (auto* runNode = dynamic_cast<RunNode*>(node)) {
            eventNodes.push_back(runNode);
        }
        if (auto* tickNode = dynamic_cast<TickNode*>(node)) {
            tickNodes.push_back(tickNode);
        }
        if (auto* timerNode = dynamic_cast<TimerNode*>(node)) {
            timerNodes.push_back(timerNode);
        }
        if (auto* mapperNode = dynamic_cast<MapperNode*>(node)) {
            mapperNodes.push_back(mapperNode);
        }
    }

    void Graph::removeNode(GraphNode* node) {
        if (!node) return;
        for (auto it = links.begin(); it != links.end();) {
            if ((*it)->getSourceNode() == node || (*it)->getTargetNode() == node) {
                delete* it;
                it = links.erase(it);
            }
            else {
                ++it;
            }
        }
        for (auto it = nodes.begin(); it != nodes.end(); ++it) {
            if (*it == node) {
                delete* it;
                nodes.erase(it);
                break;
            }
        }
    }

    bool Graph::connect(GraphNode* source, const std::string& outputName, GraphNode* target, const std::string& inputName) {
        if (!source || !target || source == target) {
            return false;
        }

        Output* output = source->getOutput(outputName);
        if (!output) {
            return false;
        }

        Input* input = nullptr;
        if (!inputName.empty()) {
            input = target->getInput(inputName);
        }
        else {
            for (auto* in : target->getInputs()) {
                if (in && in->getType() == output->getType()) {
                    if (output->getType() == IOType::EXECUTION && in->getName() != "Execute") continue;
                    input = in;
                    break;
                }
            }
        }

        if (!input || output->getType() != input->getType()) {
            return false;
        }

        for (auto* link : links) {
            if (link->getOutput() == output && link->getTargetInput() == input) {
                return false;
            }
        }

        Link* link = new Link(output, target, input);
        if (output->getType() != IOType::EXECUTION) {
            link->transferData();
        }
        links.push_back(link);
        std::cout << "[Graph] Conectado " << source->getName() << ":" << output->getName() << " a " << target->getName() << ":" << input->getName() << "\n";
        return true;
    }

    void Graph::executeFrom(GraphNode* startNode) {
        if (!startNode) {
            std::cerr << "[Graph] Error: se intentó ejecutar desde un nodo nulo.\n";
            return;
        }

        std::queue<GraphNode*> executionQueue;
        executionQueue.push(startNode);

        while (!executionQueue.empty()) {
            GraphNode* currentNode = executionQueue.front();
            executionQueue.pop();
            if (!currentNode) continue;

            currentNode->execute(executionQueue);
        }
    }

    void Graph::update(float dt) {

        for (auto* node : mapperNodes) {
            node->update(dt);
        }

        for (auto* node : tickNodes) {
            if (auto* tickNode = dynamic_cast<TickNode*>(node)) {
                tickNode->update(dt);
                if (tickNode->isRunning()) {
                    this->executeFrom(tickNode);
                }
            }
        }
       
        for (auto* node : timerNodes) {
            if (auto* timerNode = dynamic_cast<TimerNode*>(node)) {
                if (timerNode->isWaiting()) { 
                    timerNode->update(dt);
                    if (!timerNode->isWaiting()) { 
                        std::cout << "[Graph] TimerNode '" << timerNode->getName() << "' completed. Propagating execution.\n";
                        for (auto* outputPin : timerNode->getOutputs()) {
                            if (outputPin->getType() == IOType::EXECUTION) {
                                for (auto* link : outputPin->getLinks()) {
                                    if (auto* nextNode = link->getTargetNode()) {
                                        this->executeFrom(nextNode); 
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void Graph::clear() {

        while (!links.empty()) {
            delete links.front();
            links.pop_front();
        }

        while (!nodes.empty()) {
            delete nodes.front();
            nodes.pop_front();
        }

        eventNodes.clear();
        tickNodes.clear();
        timerNodes.clear();
    }
}
