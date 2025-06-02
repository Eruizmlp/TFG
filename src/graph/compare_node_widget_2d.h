#pragma once

#include "node_widget_2d.h"
#include "compare_node.h"

namespace GraphSystem {

    class CompareNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        bool inspectorVisible = false;
        CompareNode* compareNode = nullptr;

        void initInspector();

    public:
        CompareNodeWidget2D(const std::string& nodeType,
            CompareNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
    };

}
