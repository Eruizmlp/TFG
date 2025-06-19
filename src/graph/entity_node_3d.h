#pragma once
#include "graph_node.h"
#include "io.h"
#include "framework/nodes/mesh_instance_3d.h"
#include <queue>

namespace GraphSystem {

    class EntityNode3D : public GraphNode {
        MeshInstance3D* entity = nullptr;
        Output* outputEntity = nullptr;

        std::string entity_name_on_load;
        
    public:
        explicit EntityNode3D(const std::string& name);

        void setEntity(MeshInstance3D* e);
        MeshInstance3D* getEntity() const;

        void setEntityNameOnLoad(const std::string& name) { entity_name_on_load = name; }
        const std::string& getEntityNameOnLoad() const { return entity_name_on_load; } 

        void serialize(std::ofstream& binary_scene_file) override; 
        void parse(std::ifstream& binary_scene_file) override;   
        void execute(std::queue<GraphNode*>& executionQueue) override;

    };

}
