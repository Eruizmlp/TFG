#include "graph_node3D.h"
#include <iostream>

GraphNode3D::GraphNode3D(const std::string& name, GraphSystem::Graph* graph)
    : GraphSystem::GraphNode(name),  
    Node3D(),                     
    graph(graph),
    inputTransform(addInput("Transform", GraphSystem::IOType::TRANSFORM)),
    outputTransform(addOutput("Transform", GraphSystem::IOType::TRANSFORM))
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
    if (!isExecutionPending()) {
        return;
    }

    std::cout << "[GraphNode3D] Executing: " << getName() << "\n";

    // Get current transform once
    const Transform currentTransform = getTransform();

    // Propagate current transform through dedicated output
    if (outputTransform && outputTransform->getType() == GraphSystem::IOType::TRANSFORM) {
        try {
            outputTransform->setData<Transform>(currentTransform);
        }
        catch (const std::exception& e) {
            std::cerr << "[Error] Failed to set transform data: " << e.what() << "\n";
        }
    }

    // Process all other outputs
    for (auto& output : getOutputs()) {
        if (!output || output == outputTransform) {
            continue;
        }

        // Handle different output types
        switch (output->getType()) {
        case GraphSystem::IOType::EXECUTION:
            // For execution outputs, trigger connected nodes
            for (auto& link : output->getLinks()) {
                if (link && link->getTargetNode()) {
                    link->getTargetNode()->setExecutionPending(true);
                }
            }
            break;
            
        case GraphSystem::IOType::TRANSFORM:
            // Secondary transform output
            try {
                output->setData<Transform>(currentTransform);
            }
            catch (const std::exception& e) {
                std::cerr << "[Error] Failed to set secondary transform data: " << e.what() << "\n";
            }
            break;

        default:
            std::cerr << "[Warning] Unhandled output type ("
                << static_cast<int>(output->getType())
                << ") in GraphNode3D\n";
            break;
        }
    }

    setExecutionPending(false);
}

void GraphNode3D::render() {
    Node3D::render();
}
