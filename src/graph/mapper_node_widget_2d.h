#pragma once

#include "node_widget_2d.h"
#include "mapper_node.h"

namespace GraphSystem {

    class MapperNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;
        MapperNode* mapperNode = nullptr;

        void initInspector();

    public:
        MapperNodeWidget2D(const std::string& nodeType,
            MapperNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
    };

}
