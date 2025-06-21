#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"
#include <queue>
#include <fstream>

namespace GraphSystem {
    class ScaleNode : public GraphNode {
    public:
        explicit ScaleNode(const std::string& name, float factor = 1.0f);

        void setScaleFactor(float f);
        float getScaleFactor() const;

        void execute(std::queue<GraphNode*>& executionQueue) override;

       
        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
      
        float factor; 

        // Punteros a los pines
        Input* execInput = nullptr;
        Input* transformInput = nullptr;
        Input* factorInput = nullptr;
        Output* execOutput = nullptr;
        Output* transformOutput = nullptr;
    };
}
