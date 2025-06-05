#pragma once
#include "node_widget_2d.h"
#include "math_node.h"

namespace GraphSystem {

    class MathNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;

        // UI
        ui::Button2D* floatBtn = nullptr;
        ui::Button2D* vec3Btn = nullptr;
        ui::Button2D* addBtn = nullptr;
        ui::Button2D* subBtn = nullptr;
        ui::Button2D* mulBtn = nullptr;
        ui::Button2D* divBtn = nullptr;

    public:
        MathNodeWidget2D(const std::string& nodeType,
            MathNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void update(float delta_time) override;

    private:
        void initInspector();
        void updateInspector();
    };

} // namespace GraphSystem
