#pragma once

#include "graph_node.h"
#include "node_widget_2d.h"


namespace GraphSystem {

    class TickNode;

    class TickNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        ui::Text2D* statusLabel = nullptr;
        ui::Button2D* toggleButton = nullptr;
        bool inspectorVisible = false;



    public:
        TickNodeWidget2D(const std::string& nodeType,
            TickNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void update(float delta_time) override;

    private:
        void initInspector();
        void updateInspector();
    };

} 
