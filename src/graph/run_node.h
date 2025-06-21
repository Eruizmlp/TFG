#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream>
#include <string>

namespace GraphSystem {

    class RunNode : public GraphNode {
    public:
        explicit RunNode(const std::string& name);
        ~RunNode() override;

        void execute(std::queue<GraphNode*>& executionQueue) override;
        const std::string& getEventName() const { return eventName; }

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
        std::string eventName;
        Output* execOutput = nullptr; 
    };

}
