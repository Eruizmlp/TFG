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


    class NodeWidget2D : public ui::Panel2D {
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

       
        bool on_input(sInputData data) override;
    };

}
