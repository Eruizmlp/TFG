#include "entity_node_3d.h"
#include <sstream>
#include <fstream>
#include <spdlog/spdlog.h>

namespace GraphSystem {

    EntityNode3D::EntityNode3D(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        outputEntity = addOutput("Mesh", IOType::MESH); 
        outputEntity->setComputeFunction([this]() -> VariableValue { 
            return VariableValue(entity); 
            });

        outputEntity->setData(VariableValue(static_cast<MeshInstance3D*>(nullptr))); 
    }

    void EntityNode3D::setEntity(MeshInstance3D* e) {
        entity = e; 
        if (outputEntity) {
            outputEntity->setData(VariableValue(entity)); 
        }
    }

    MeshInstance3D* EntityNode3D::getEntity() const {
        return entity; 
    }

    void EntityNode3D::execute(std::queue<GraphNode*>& executionQueue)
    {
    }

    void EntityNode3D::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
        std::string linked_entity_name = (entity) ? entity->get_name() : "";
        uint64_t name_size = linked_entity_name.size();
        file.write(reinterpret_cast<const char*>(&name_size), sizeof(name_size));
        file.write(linked_entity_name.c_str(), name_size);
    }

    void EntityNode3D::parse(std::ifstream& file) {
        GraphNode::parse(file);
        uint64_t name_size = 0;
        file.read(reinterpret_cast<char*>(&name_size), sizeof(name_size));
        entity_name_on_load.resize(name_size);
        if (name_size > 0) {
            file.read(&entity_name_on_load[0], name_size);
        }
    }

    void EntityNode3D::rebindPins() {
        outputEntity = getOutput("Mesh");
        if (!outputEntity) {
            spdlog::error("EntityNode3D '{}': Could not rebind 'Mesh' output pin.", getName());
        }
    }

}
