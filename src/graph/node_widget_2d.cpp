#include "node_widget_2d.h"
#include "port_button_2d.h"
#include "framework/ui/io.h"  


using namespace GraphSystem;
using namespace ui;

glm::vec2 NodeWidget2D::computeSize(GraphNode* node) {
    constexpr float WIDTH = 200.0f;
    constexpr float ROW_H = 20.0f;
    constexpr float MARGIN = 4.0f;

    size_t numPorts = node->getOutputs().size()
        + node->getInputs().size();
    size_t rows = 2 + numPorts;  

    float height = MARGIN + rows * (ROW_H + MARGIN);
    return { WIDTH, height };
}

NodeWidget2D::NodeWidget2D(GraphNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : Panel2D("Widget2D_" + node->getName(),
        { 0,0 },
        computeSize(node),
        1u,
        colors::GRAY),
    logic_node(node),
    graphEditor(editor)
{
    // Posicionar el panel
    set_position({ worldPos.x, worldPos.y });

    // Crear container
    rootContainer = new VContainer2D(
        "NodeRoot_" + node->getName(),
        { 0,0 }, 0u, colors::GRAY
    );
    rootContainer->padding = { 8,8 };
    rootContainer->item_margin = { 4,4 };
    rootContainer->set_fixed_size(get_size());
    add_child(rootContainer);

    // Título
    {
        auto* title = new Text2D(node->getName(), { 0,0 });
        title->set_color(colors::WHITE);
        rootContainer->add_child(title);
    }

    // Puertos OUTPUT
    for (auto* outPort : logic_node->getOutputs()) {
        auto* row = new HContainer2D("RowOut_" + outPort->getName(), { 0,0 }, 0u, colors::GRAY);
        row->padding = { 4,2 };
        row->item_margin = { 8,0 };
        rootContainer->add_child(row);

        auto* lbl = new Text2D(outPort->getName(), { 0,0 });
        lbl->set_color(colors::GREEN);
        row->add_child(lbl);

        sButtonDescription desc;
        desc.size = { 16,16 };
        desc.color = colors::GREEN;
        auto* btn = new PortButton2D(
            "outBtn_" + node->getName() + "_" + outPort->getName(),
            desc,
            logic_node,
            graphEditor,
            outPort->getName(),
            true
        );
        row->add_child(btn);
    }

    // Puertos INPUT
    for (auto* inPort : logic_node->getInputs()) {
        auto* row = new HContainer2D("RowIn_" + inPort->getName(), { 0,0 }, 0u, colors::GRAY);
        row->padding = { 4,2 };
        row->item_margin = { 8,0 };
        rootContainer->add_child(row);

        sButtonDescription desc;
        desc.size = { 16,16 };
        desc.color = colors::RED;
        auto* btn = new PortButton2D(
            "inBtn_" + node->getName() + "_" + inPort->getName(),
            desc,
            logic_node,
            graphEditor,
            inPort->getName(),
            false
        );
        row->add_child(btn);

        auto* lbl = new Text2D(inPort->getName(), { 0,0 });
        lbl->set_color(colors::RED);
        row->add_child(lbl);
    }

}

//TODO Drag de los nodos
bool NodeWidget2D::on_input(sInputData data) {
    // Al presionar, capturo ratón y empiezo drag
    if (data.is_pressed && !dragging) {
        ::IO::set_focus(this);
        ::IO::set_want_capture_input(true);
        dragging = true;
        dragOffset = get_local_translation() - data.local_position;
        return true;
    }
    // Mientras mantengo pulsado, muevo el nodo
    if (dragging && data.is_pressed) {
        set_position(data.local_position + dragOffset);
        return true;
    }
    // Al soltar, termino drag y devuelvo el ratón
    if (dragging && data.was_released) {
        dragging = false;
        ::IO::blur();
        ::IO::set_want_capture_input(false);
        return true;
    }
    // Si no estoy en drag, delego al Panel2D y a los botones hijos
    return Panel2D::on_input(data);
}
