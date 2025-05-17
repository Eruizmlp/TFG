#pragma once
#include "node_widget_2d.h"
#include "math_node.h"


namespace GraphSystem {

    class MathNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;
        MathNode* mathNode = nullptr;

        void initInspector();

    public:
        MathNodeWidget2D(MathNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
    };

}
