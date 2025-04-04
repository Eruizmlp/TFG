#ifndef STARTNODE_H
#define STARTNODE_H

#include "graph_node.h"

namespace GraphSystem {
    class StartNode : public GraphNode {
    public:
        explicit StartNode(const std::string& name);
        void execute() override;
    };
}

#endif // STARTNODE_H
