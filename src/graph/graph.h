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
        std::list<Link*> links;
        bool m_isTickGraph;
        std::string m_name;

    public:
        explicit Graph(const std::string& name, bool isTickGraph = false);
        ~Graph();

        const std::string& getName() const { return m_name; }
        bool isTickGraph() const { return m_isTickGraph; }

        void addNode(GraphNode* node);
        void removeNode(GraphNode* node);
        bool connect(GraphNode* source, const std::string& outputName,
            GraphNode* target, const std::string& inputName = "");

        void render();
        void execute();

        const std::list<GraphNode*>& getNodes() const { return nodes; }
        const std::list<Link*>& getLinks() const { return links; }

        void clear();
    };

} // namespace GraphSystem

#endif // GRAPH_H
