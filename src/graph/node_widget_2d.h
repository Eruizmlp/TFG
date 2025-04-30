// node_widget_2d.h
#pragma once

#include "framework/nodes/panel_2d.h"
#include "framework/nodes/node_2d.h"
#include "framework/nodes/container_2d.h"
#include "framework/nodes/text.h"
#include "framework/nodes/button_2d.h"
#include "framework/colors.h"

#include "graph_node.h"
#include "graph_editor.h"
#include "port_button_2d.h"

namespace GraphSystem {

    // forward‐declare the helper
    class DragHandle2D;

    class NodeWidget2D : public ui::Panel2D {
        friend class DragHandle2D;   // allow it to touch our private drag state

    private:
        GraphNode* logic_node;
        GraphEditor* graphEditor;
        ui::VContainer2D* rootContainer;

        bool        dragging = false;
        glm::vec2   dragOffset;

        static glm::vec2 computeSize(GraphNode* node);

    public:
        NodeWidget2D(GraphNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        // we still override this so the panel's full rect is hit‐tested first
        sInputData get_input_data(bool ignore_focus = false) override;

        // now on_input only handles focus/dbl‐click etc.
        bool on_input(sInputData data) override;
        void update(float delta_time) override;

    };

} // namespace GraphSystem
