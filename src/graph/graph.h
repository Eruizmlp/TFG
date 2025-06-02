#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <string>
#include "graph_node.h"
#include "link.h"

namespace GraphSystem {

    class Graph {
    private:
        std::list<GraphNode*> nodes;
        std::list<Link*>      links;
        bool                  m_isTickGraph;
        std::string           m_name;

    public:
        explicit Graph(const std::string& name);
        ~Graph();

        const std::string& getName() const { return m_name; }
        void setName(const std::string& newName) { m_name = newName; }

        void addNode(GraphNode* node);
        void removeNode(GraphNode* node);
        bool connect(GraphNode* source,
            const std::string& outputName,
            GraphNode* target,
            const std::string& inputName = "");

        void render();
        void execute();
        void update(float dt);

        void triggerEntryPoints();

        const std::list<GraphNode*>& getNodes() const { return nodes; }
        const std::list<Link*>& getLinks() const { return links; }

        void clear();
    };

} 

#endif 
