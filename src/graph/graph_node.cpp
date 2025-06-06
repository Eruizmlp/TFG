#include "graph_node.h"
#include <iostream>
#include <fstream>

namespace GraphSystem {

    GraphNode::GraphNode(const std::string& name, NodeCategory category)
        : m_name(name), m_isEntryPoint(false), m_executionPending(false), m_category(category)
    {
    }

    GraphNode::~GraphNode() {
        for (auto input : m_inputs) delete input;
        for (auto output : m_outputs) delete output;
        removeAllLinks();
    }

    Input* GraphNode::addInput(const std::string& name, IOType type) {
        Input* input = new Input(name, type);
        m_inputs.push_back(input);
        return input;
    }

    Output* GraphNode::addOutput(const std::string& name, IOType type) {
        std::cout << "[GraphNode] Adding output: " << name << " Type: " << static_cast<int>(type) << "\n";
        Output* output = new Output(this, name, type);
        m_outputs.push_back(output);
        return output;
    }

    Link* GraphNode::createLink(Output* source, GraphNode* target) {
        if (!source || !target) return nullptr;
        Link* link = new Link(source, target);
        source->addLink(link);
        return link;
    }

    void GraphNode::removeLink(Link* link) {
        if (!link) return;
        if (Output* output = link->getOutput()) {
            output->removeLink(link);
        }
        delete link;
    }

    void GraphNode::removeAllLinks() {
        for (auto output : m_outputs) {
            for (auto link : output->getLinks()) {
                delete link;
            }
            output->getLinks().clear();
        }
    }

    Input* GraphNode::getInput(const std::string& name) {
        for (auto input : m_inputs) {
            if (input && input->getName() == name) {
                return input;
            }
        }
        return nullptr;
    }

    const Input* GraphNode::getInput(const std::string& name) const {
        for (auto input : m_inputs) {
            if (input && input->getName() == name) {
                return input;
            }
        }
        return nullptr;
    }

    Output* GraphNode::getOutput(const std::string& name) {
        for (auto* output : m_outputs) {
            if (output && output->getName() == name) {
                return output;
            }
        }
        return nullptr;
    }

    void GraphNode::execute(std::queue<GraphNode*>& executionQueue) {

    }

    const Output* GraphNode::getOutput(const std::string& name) const {
        for (const auto* output : m_outputs) {
            if (output && output->getName() == name) {
                return output;
            }
        }
        return nullptr; 
    }

    std::list<Link*> GraphNode::getLinks() const {
        std::list<Link*> links;
        for (auto output : m_outputs) {
            links.insert(links.end(),
                output->getLinks().begin(),
                output->getLinks().end());
        }
        return links;
    }

   
    void GraphNode::serialize(std::ofstream& binary_scene_file) {

        sGraphNodeBinaryHeader header = {
            .input_count = m_inputs.size(),
            .output_count = m_outputs.size(),
        };

        binary_scene_file.write(reinterpret_cast<char*>(&header), sizeof(sGraphNodeBinaryHeader));

        binary_scene_file.write(reinterpret_cast<char*>(&m_category), sizeof(size_t));

        size_t name_size = m_name.size();
        binary_scene_file.write(reinterpret_cast<char*>(&name_size), sizeof(size_t));
        binary_scene_file.write(m_name.c_str(), name_size);
    }

    void GraphNode::parse(std::ifstream& binary_scene_file) {

        sGraphNodeBinaryHeader header;
        binary_scene_file.read(reinterpret_cast<char*>(&header), sizeof(sGraphNodeBinaryHeader));

        binary_scene_file.read(reinterpret_cast<char*>(&m_category), sizeof(uint64_t));

        uint64_t name_size = 0;
        binary_scene_file.read(reinterpret_cast<char*>(&name_size), sizeof(uint64_t));
        m_name.resize(name_size);
        binary_scene_file.read(&m_name[0], name_size);
    }
}
