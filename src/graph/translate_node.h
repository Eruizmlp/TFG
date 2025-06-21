#pragma once
#include "graph_node.h"
#include "framework/nodes/mesh_instance_3d.h"
#include <glm/glm.hpp>
#include <queue>
#include <fstream>

namespace GraphSystem {
    class TranslateNode : public GraphNode {
    public:
        explicit TranslateNode(const std::string& name, const glm::vec3& offset = glm::vec3(0.0f));

        void setOffset(const glm::vec3& offset);
        glm::vec3 getOffset() const;

        void execute(std::queue<GraphNode*>& executionQueue) override;

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
      
        glm::vec3 offset; 

        Input* execInput = nullptr;
        Input* transformInput = nullptr;
        Input* offsetInput = nullptr;
        Output* execOutput = nullptr;
        Output* transformOutput = nullptr;
    };
}
