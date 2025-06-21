#pragma once

#include "node_widget_2d.h"
#include "set_color_node.h"

namespace GraphSystem {

   
    class SetColorNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;
        SetColorNode* setColorNode = nullptr;

        ui::FloatSlider2D* rSlider = nullptr;
        ui::FloatSlider2D* gSlider = nullptr;
        ui::FloatSlider2D* bSlider = nullptr;
        ui::FloatSlider2D* aSlider = nullptr;

        ui::Text2D* rValueLabel = nullptr;
        ui::Text2D* gValueLabel = nullptr;
        ui::Text2D* bValueLabel = nullptr;
        ui::Text2D* aValueLabel = nullptr;

        void initInspector();

    public:
        SetColorNodeWidget2D(const std::string& nodeType,
            SetColorNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
    };

}
