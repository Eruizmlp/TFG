#pragma once
#include "framework/nodes/panel_2d.h"
#include "../graph/graph_node.h"
#include "../graph/graph_editor.h"


namespace GraphSystem {

    class PortButton2D; // ya descrito antes

    /// Un panel 2D que vive en la escena 3D y muestra el nodo + sus puertos
    class NodeWidget2D : public ui::Panel2D {
    private:
        GraphNode* logic_node;
        GraphEditor* graphEditor;
        std::vector<PortButton2D*> portButtons;

    public:
        /// worldPos: posición en el espacio donde se colocará el panel 2D
        NodeWidget2D(GraphNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        GraphNode* getLogicNode() const { return logic_node; }
    private:
        static glm::vec2 computeSize(GraphNode* node);
    };

}
