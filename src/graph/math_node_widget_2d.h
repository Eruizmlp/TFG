#pragma once

#include "node_widget_2d.h"
#include "math_node.h"

namespace GraphSystem {

    class MathNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;
        GraphSystem::MathNode* mathNode = nullptr;

        
        ui::Button2D* plusBtn = nullptr;
        ui::Button2D* minusBtn = nullptr;
        ui::Button2D* mulBtn = nullptr;
        ui::Button2D* divBtn = nullptr;

    public:
        MathNodeWidget2D(
            const std::string& nodeType,
            GraphSystem::MathNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void update(float delta_time) override;
        void toggleInspector(sInputData data) override;
        void initInspector();
    };
}
