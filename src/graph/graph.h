#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <vector>
#include <string>
#include "graph_node.h"
#include "link.h"

namespace GraphSystem {

    class Graph {
    private:
        std::list<GraphNode*> nodes;
        std::list<Link*> links;
        std::string m_name;

        std::vector<GraphNode*> eventNodes;  
        std::vector<GraphNode*> tickNodes;   

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

        void triggerEventNodes();  

        const std::list<GraphNode*>& getNodes() const { return nodes; }
        const std::list<Link*>& getLinks() const { return links; }

        void clear();
    };

} 

#endif 
