#pragma once

#include "graph_node.h"
#include "io.h"

namespace GraphSystem {

    class RunNode : public GraphNode {
    public:
        explicit RunNode(const std::string& name);
        virtual ~RunNode() override;  

        void execute() override;
        const std::string& getEventName() const { return eventName; }

    private:
        std::string eventName;
        Output* execOutput;
    };

} 
