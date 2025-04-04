#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <list>
#include <string>
#include "io.h"
#include "link.h"

namespace GraphSystem {

    class GraphNode {
    protected:
        std::string m_name;
        std::list<Input*> m_inputs;
        std::list<Output*> m_outputs;
        bool m_isEntryPoint;
        bool m_executionPending;

    public:
        explicit GraphNode(const std::string& name);
        GraphNode() : GraphNode("UnnamedNode") {}

        virtual ~GraphNode();

        // Node identification
        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        // Input/Output management
        Input* addInput(const std::string& name, IOType type);
        Output* addOutput(const std::string& name, IOType type);

        // Link management
        Link* createLink(Output* source, GraphNode* target);
        void removeLink(Link* link);
        void removeAllLinks();

        // Node operations
        virtual void execute();
        virtual void render();

        // Getters
        Input* getInput(const std::string& name);
        const Input* getInput(const std::string& name) const;
        const std::list<Input*>& getInputs() const { return m_inputs; }
        const std::list<Output*>& getOutputs() const { return m_outputs; }
        std::list<Link*> getLinks() const;

        bool isEntryPoint() const { return m_isEntryPoint; }
        void setEntryPoint(bool isEntry) { m_isEntryPoint = isEntry; }

        bool isExecutionPending() const { return m_executionPending; }
        void setExecutionPending(bool pending) { m_executionPending = pending; }
    };

} // namespace GraphSystem

#endif // GRAPH_NODE_H
