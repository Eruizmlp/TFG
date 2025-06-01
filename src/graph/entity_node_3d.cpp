#include "entity_node_3d.h"

namespace GraphSystem {

    EntityNode3D::EntityNode3D()
        : GraphNode("Entity3D", NodeCategory::DATA)
    {
        outputEntity = addOutput("Mesh", IOType::MESH);

        outputEntity->setComputeFunction([this]() -> MeshInstance3D* {
            return entity;
            });
    }

    void EntityNode3D::setEntity(MeshInstance3D* e) {
        entity = e;
        if (outputEntity) {
            outputEntity->setData(entity);  
        }
    }

    MeshInstance3D* EntityNode3D::getEntity() const {
        return entity;
    }

}
