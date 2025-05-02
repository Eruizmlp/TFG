#pragma once

#include <glm/glm.hpp>

#include "framework/nodes/panel_2d.h"
#include "framework/nodes/node_2d.h"
#include "framework/nodes/container_2d.h"
#include "framework/nodes/text.h"
#include "framework/nodes/button_2d.h"
#include "framework/nodes/slider_2d.h"
#include "framework/colors.h"

#include "graph_node.h"
#include "graph_editor.h"
#include "port_button_2d.h"
#include "rotate_node.h"

namespace GraphSystem {

    class NodeWidget2D : public ui::Panel2D {
    protected:
        GraphNode* logic_node;
        GraphEditor* graphEditor;
        ui::VContainer2D* rootContainer;
        bool              dragging = false;
        glm::vec2         dragOffset;
        static glm::vec2  computeSize(GraphNode* node);

    public:
        NodeWidget2D(GraphNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);
        GraphNode* getLogicNode() const { return logic_node; }
        sInputData get_input_data(bool ignore_focus = false) override;
        bool       on_input(sInputData data) override;
        void       update(float delta_time) override;

        virtual void toggleInspector(sInputData data) {}
        virtual void updateInspector() {}
    };

    class RotateNodeWidget2D : public NodeWidget2D {
    private:
        ui::Panel2D* inspectPanel = nullptr;
        ui::FloatSlider2D* angleSlider = nullptr;
        ui::Button2D* axisXBtn = nullptr;
        ui::Button2D* axisYBtn = nullptr;
        ui::Button2D* axisZBtn = nullptr;
        bool               inspectorVisible = false;
        void initInspector();
    public:
        RotateNodeWidget2D(RotateNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);
        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;
    };

}
