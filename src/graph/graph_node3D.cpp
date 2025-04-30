#include "graph_node3D.h"
#include <iostream>

namespace GraphSystem{

    GraphNode3D::GraphNode3D(const std::string& name, GraphSystem::Graph* graph)
        : GraphSystem::GraphNode(name),  
        Node3D(),                     
        graph(graph),
        inputTransform(addInput("TransformIn", GraphSystem::IOType::TRANSFORM)),
        outputTransform(addOutput("TransformOut", GraphSystem::IOType::TRANSFORM))
    {
  
    }

    void GraphNode3D::setGraph(GraphSystem::Graph* graph) {
        this->graph = graph;
    }

    GraphSystem::Graph* GraphNode3D::getGraph() const {
        return graph;
    }

    void GraphNode3D::setTransform(const Transform& newTransform) {
        Node3D::set_transform(newTransform);
        if (outputTransform) {
            outputTransform->setData(newTransform);
            std::cout << "[GraphNode3D] Transform updated and propagated: " << getName() << "\n";
        }
    }

    Transform& GraphNode3D::getTransform() {
        return Node3D::get_transform();
    }

    void GraphNode3D::update(float delta_time) {
        Node3D::update(delta_time);
    }

    void GraphNode3D::execute() {
        if (!isExecutionPending()) return;

        if (inputTransform && inputTransform->hasData()) {
            set_transform(inputTransform->getTransform());
        }

        const Transform current = get_transform();
        if (outputTransform) {
            outputTransform->setData(current);
        }

        for (auto& out : getOutputs()) {
            if (out->getType() == IOType::EXECUTION) {
                for (auto& link : out->getLinks()) {
                    if (auto tgt = link->getTargetNode()) {
                        tgt->setExecutionPending(true);
                    }
                }
            }
        }

        setExecutionPending(false);
    }


    void GraphNode3D::render() {
        Node3D::render();
    }

}
