#include "node_widget_2d.h"
#include "port_button_2d.h"    

using namespace GraphSystem;

glm::vec2 NodeWidget2D::computeSize(GraphNode* node) {
    const float margin = 6.0f;
    const float btnSize = 20.0f;
    const float fixedWidth = 140.0f;

    // calculamos cuántos puertos hay (max entre in/outs)
    size_t maxPorts = std::max(
        node->getOutputs().size(),
        node->getInputs().size()
    );
    // Altura necesaria: un margen arriba, un botón + margen por puerto
    float totalHeight = margin + maxPorts * (btnSize + margin);

    return glm::vec2(fixedWidth, totalHeight);
}

NodeWidget2D::NodeWidget2D(GraphNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : ui::Panel2D(
        "Widget2D_" + node->getName(),
        glm::vec2(0.0f),
        computeSize(node),  
        1u,
        colors::GRAY
    ),
    logic_node(node),
    graphEditor(editor)
{

    set_position(worldPos);

    const float margin = 6.0f;
    const float btnSize = 20.0f;


    glm::vec2 size = get_size();

    size_t outIndex = 0;
    for (auto* outPort : logic_node->getOutputs()) {
        ui::sButtonDescription desc;
        desc.label = "";
        desc.size = glm::vec2(btnSize, btnSize);
        desc.position = glm::vec2(
            size.x - btnSize - margin,                
            margin + outIndex * (btnSize + margin)    
        );
        desc.color = colors::GREEN;

        auto* btn = new PortButton2D(
            "out_" + node->getName() + "_" + outPort->getName(),
            desc,
            logic_node,
            graphEditor,
            outPort->getName(),
            true
        );
        add_child(btn);
        portButtons.push_back(btn);
        ++outIndex;
    }

    size_t inIndex = 0;
    for (auto* inPort : logic_node->getInputs()) {
        ui::sButtonDescription desc;
        desc.label = "";
        desc.size = glm::vec2(btnSize, btnSize);
        desc.position = glm::vec2(
            margin,                                  
            margin + inIndex * (btnSize + margin)    
        );
        desc.color = colors::RED;

        auto* btn = new PortButton2D(
            "in_" + node->getName() + "_" + inPort->getName(),
            desc,
            logic_node,
            graphEditor,
            inPort->getName(),
            false
        );
        add_child(btn);
        portButtons.push_back(btn);
        ++inIndex;
    }
}
