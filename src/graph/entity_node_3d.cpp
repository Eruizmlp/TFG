#include "entity_node_3d.h"
#include <sstream>
#include <fstream>

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

    void EntityNode3D::serialize(std::ofstream& binary_scene_file) {
        GraphNode::serialize(binary_scene_file);

        std::string linked_entity_name = (entity) ? entity->get_name() : "";
        uint64_t name_size = linked_entity_name.size();
        binary_scene_file.write(reinterpret_cast<char*>(&name_size), sizeof(uint64_t));
        binary_scene_file.write(linked_entity_name.c_str(), name_size);
    }

    void EntityNode3D::parse(std::ifstream& binary_scene_file) {
     
        GraphNode::parse(binary_scene_file);

        
        uint64_t name_size = 0;
        binary_scene_file.read(reinterpret_cast<char*>(&name_size), sizeof(uint64_t));
        entity_name_on_load.resize(name_size);
        binary_scene_file.read(&entity_name_on_load[0], name_size);
    }


}
