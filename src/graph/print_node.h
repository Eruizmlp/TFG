#ifndef PRINTNODE_H
#define PRINTNODE_H

#include "graph_node.h"
#include <iostream>

namespace GraphSystem {

    class PrintNode : public GraphSystem::GraphNode {
    public:
        explicit PrintNode(const std::string& name);
        void execute() override;
    };
}


#endif // PRINTNODE_H
