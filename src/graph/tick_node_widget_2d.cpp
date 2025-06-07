#include "tick_node_widget_2d.h"
#include "tick_node.h"
#include <framework/input.h>
#include <framework/colors.h>

using namespace GraphSystem;

TickNodeWidget2D::TickNodeWidget2D(
    const std::string& nodeType,
    TickNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(nodeType, node, editor, worldPos)
{

}

void TickNodeWidget2D::toggleInspector(sInputData data) {
    if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void TickNodeWidget2D::update(float delta_time) {
    toggleInspector(get_input_data(false));
    NodeWidget2D::update(delta_time);

    updateInspector();

    if (inspectPanel && inspectorVisible) {
        inspectPanel->update(delta_time);
    }
}

void TickNodeWidget2D::initInspector() {
    glm::vec2 pos = glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 80.0f };

    inspectPanel = new ui::XRPanel("TickInspect_" + logic_node->getName(), pos, sz, 0u, colors::YELLOW);
    auto* tickNode = static_cast<TickNode*>(logic_node);

    statusLabel = new ui::Text2D("State: Stopped", { 8, 10 });
    inspectPanel->add_child(statusLabel);

    auto* toggleRow = new ui::HContainer2D("ToggleRow", { 8, 30 }, 0u, colors::GRAY);
    toggleRow->item_margin = { 4, 0 };
    inspectPanel->add_child(toggleRow);

    ui::sButtonDescription bdesc;
    bdesc.size = { 24, 24 };
    bdesc.color = colors::WHITE;

    auto* onButton = new ui::Button2D("TickOn_" + logic_node->getName(), bdesc);
    onButton->add_child(new ui::Text2D("ON", { 0, 0 }));
    auto* offButton = new ui::Button2D("TickOff_" + logic_node->getName(), bdesc);
    offButton->add_child(new ui::Text2D("OFF", { 0, 0 }));
    toggleRow->add_child(onButton);
    toggleRow->add_child(offButton);

    Node::bind(onButton->get_name(), FuncVoid([tickNode](const std::string&, void*) {
        tickNode->start();
        }));

    Node::bind(offButton->get_name(), FuncVoid([tickNode](const std::string&, void*) {
        tickNode->stop();
        }));

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}

void TickNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible || !statusLabel) return;

    auto* tickNode = static_cast<TickNode*>(logic_node);
    if (tickNode->isRunning()) {
        statusLabel->set_text("State: Running");
        statusLabel->set_color(colors::GREEN);
    }
    else {
        statusLabel->set_text("State: Stopped");
        statusLabel->set_color(colors::RED);
    }
}
