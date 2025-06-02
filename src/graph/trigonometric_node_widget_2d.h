#pragma once

#include "node_widget_2d.h"
#include "trigonometric_node.h"

namespace GraphSystem {

    class TrigonometricNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;
        TrigonometricNode* trigNode = nullptr;

        void initInspector();

    public:
        TrigonometricNodeWidget2D(const std::string& nodeType,
            TrigonometricNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
    };

}
