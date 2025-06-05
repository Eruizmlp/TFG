#include "entity_node_3d.h"


namespace GraphSystem {

    EntityNode3D::EntityNode3D()
        : GraphNode("Entity3D", NodeCategory::DATA)
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

    void EntityNode3D::execute()
    {
   
    }

}
