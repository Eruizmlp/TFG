#pragma once

#include "framework/nodes/node_3d.h"
#include "../graph/graph.h"
#include "io.h"

namespace GraphSystem {
    class Graph;

    class GraphNode3D : public GraphSystem::GraphNode, public Node3D {
    private:
        GraphSystem::Graph* graph; 
        GraphSystem::Input* inputTransform = nullptr;
        GraphSystem::Output* outputTransform = nullptr;
    


    public:
        explicit GraphNode3D(const std::string& name,GraphSystem::Graph* graph = nullptr);
        ~GraphNode3D() override = default;

        void setGraph(GraphSystem::Graph* graph);
        GraphSystem::Graph* getGraph() const;

        void setTransform(const Transform& newTransform);
        Transform& getTransform();

        void update(float delta_time) override;
        void render() override;
        void execute() override;
    };
}
