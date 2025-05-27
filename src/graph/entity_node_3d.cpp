#include "entity_node_3d.h"

namespace GraphSystem {

    EntityNode3D::EntityNode3D()
        : GraphNode("Entity3D", NodeCategory::DATA)
    {
        outputMesh = addOutput("Mesh", IOType::MESH);
        outputTransform = addOutput("Transform", IOType::TRANSFORM);
    }

    void EntityNode3D::setEntity(MeshInstance3D* e) {
        entity = e;
    }

    MeshInstance3D* EntityNode3D::getEntity() const {
        return entity;
    }

    void EntityNode3D::execute() {
        if (!entity) return;

        if (outputMesh)
            outputMesh->setData(entity);

        if (outputTransform)
            outputTransform->setData(entity->get_transform());
    }

}
