#include "entity_node_3d.h"

namespace GraphSystem {

    EntityNode3D::EntityNode3D()
        : GraphNode("Entity3D", NodeCategory::DATA)
    {
        outputEntity = addOutput("Mesh", IOType::MESH);
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

    void EntityNode3D::execute() {
        if (!entity) return;

        if (outputEntity)
            outputEntity->setData(entity);

       }

}
