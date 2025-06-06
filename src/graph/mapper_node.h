#pragma once
#include "graph_node.h"
#include <queue>


namespace GraphSystem {

    class MapperNode : public GraphNode {
    public:
        MapperNode(const std::string& name);

        void update(float deltaTime) override;
        void setMappedButton(int button);
        int getMappedButton() const;

        void execute(std::queue<GraphNode*>& executionQueue) override;


    private:
        Output* isPressedOutput;
        Output* wasPressedOutput;

        int buttonCode;
    };

}
