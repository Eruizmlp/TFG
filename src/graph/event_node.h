#pragma once

#include "graph_node.h"
#include "io.h"

namespace GraphSystem {

    class EventNode : public GraphNode {
    public:
        explicit EventNode(const std::string& name);
        virtual ~EventNode() override;  // Declaration

        void execute() override;
        const std::string& getEventName() const { return eventName; }

    private:
        std::string eventName;
        Output* execOutput;
    };

} // namespace GraphSystem
