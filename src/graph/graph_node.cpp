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
    void GraphNode::serialize(std::ofstream& file) {
        sGraphNodeBinaryHeader header = { (uint64_t)m_inputs.size(), (uint64_t)m_outputs.size() };
        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        file.write(reinterpret_cast<const char*>(&m_category), sizeof(m_category));

        uint64_t name_size = m_name.size();
        file.write(reinterpret_cast<const char*>(&name_size), sizeof(name_size));
        file.write(m_name.c_str(), name_size);

        for (const auto* pin : m_inputs) {
            uint64_t pin_name_size = pin->getName().size();
            file.write(reinterpret_cast<const char*>(&pin_name_size), sizeof(pin_name_size));
            file.write(pin->getName().c_str(), pin_name_size);
            IOType pin_type = pin->getType();
            file.write(reinterpret_cast<const char*>(&pin_type), sizeof(pin_type));
        }
        for (const auto* pin : m_outputs) {
            uint64_t pin_name_size = pin->getName().size();
            file.write(reinterpret_cast<const char*>(&pin_name_size), sizeof(pin_name_size));
            file.write(pin->getName().c_str(), pin_name_size);
            IOType pin_type = pin->getType();
            file.write(reinterpret_cast<const char*>(&pin_type), sizeof(pin_type));
        }
    }


    void GraphNode::parse(std::ifstream& file) {
        sGraphNodeBinaryHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        file.read(reinterpret_cast<char*>(&m_category), sizeof(m_category));

        uint64_t name_size = 0;
        file.read(reinterpret_cast<char*>(&name_size), sizeof(name_size));
        m_name.resize(name_size);
        file.read(&m_name[0], name_size);

        for (auto* pin : m_inputs) delete pin;
        m_inputs.clear();
        for (auto* pin : m_outputs) delete pin;
        m_outputs.clear();

        for (uint64_t i = 0; i < header.input_count; ++i) {
            uint64_t pin_name_size = 0;
            file.read(reinterpret_cast<char*>(&pin_name_size), sizeof(pin_name_size));
            std::string pin_name(pin_name_size, '\0');
            file.read(&pin_name[0], pin_name_size);
            IOType pin_type;
            file.read(reinterpret_cast<char*>(&pin_type), sizeof(pin_type));
            addInput(pin_name, pin_type);
        }
        for (uint64_t i = 0; i < header.output_count; ++i) {
            uint64_t pin_name_size = 0;
            file.read(reinterpret_cast<char*>(&pin_name_size), sizeof(pin_name_size));
            std::string pin_name(pin_name_size, '\0');
            file.read(&pin_name[0], pin_name_size);
            IOType pin_type;
            file.read(reinterpret_cast<char*>(&pin_type), sizeof(pin_type));
            addOutput(pin_name, pin_type);
        }
    }
}
