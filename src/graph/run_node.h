#pragma once

#include "graph_node.h"
#include "io.h"
#include <queue>

namespace GraphSystem {

    class RunNode : public GraphNode {
    public:
        explicit RunNode(const std::string& name);
        virtual ~RunNode() override;  


        void execute(std::queue<GraphNode*>& executionQueue) override;


        const std::string& getEventName() const { return eventName; }

    private:
        std::string eventName;
        Output* execOutput;
    };

} 
