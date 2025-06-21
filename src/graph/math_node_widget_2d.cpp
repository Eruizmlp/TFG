#include "math_node_widget_2d.h"
#include "math_node.h"
#include "framework/input.h"


using namespace GraphSystem;

MathNodeWidget2D::MathNodeWidget2D(
    const std::string& nodeType,
    MathNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(nodeType, node, editor, worldPos)
{
}

void MathNodeWidget2D::toggleInspector(sInputData data) {
    if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void MathNodeWidget2D::update(float delta_time) {
    sInputData d = get_input_data(false);
    if (d.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }

    NodeWidget2D::update(delta_time);

    if (inspectPanel && inspectorVisible) {
        inspectPanel->update(delta_time);
    }
}

void MathNodeWidget2D::initInspector() {
    glm::vec2 pos = glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 200.0f, 80.0f };

    inspectPanel = new ui::XRPanel(
        "MathInspect_" + logic_node->getName(),
        pos,
        sz,
        0u,
        colors::CYAN * 0.5f
    );

    auto* mathNode = static_cast<MathNode*>(logic_node);

    float currentY = 20.0f;

    // Label
    auto* label = new ui::Text2D("Operation:", { 8, currentY });
    inspectPanel->add_child(label);
    currentY += 20.0f;

    // Buttons
    auto* operationRow = new ui::HContainer2D("OperationRow", { 8, currentY }, 0u, colors::GRAY);
    operationRow->item_margin = { 4, 0 };
    inspectPanel->add_child(operationRow);

    ui::sButtonDescription bdesc;
    bdesc.size = { 24, 24 };
    bdesc.color = colors::WHITE;

    auto createOpButton = [this, mathNode, bdesc](const std::string& label, char op) {
        auto* button = new ui::Button2D("OpButton_" + label + "_" + logic_node->getName(), bdesc);
        button->add_child(new ui::Text2D(label, { 0, 0 }));

        Node::bind(button->get_name(), FuncVoid([mathNode, op](const std::string&, void*) {
            mathNode->setOperation(op);
            }));

        return button;
        };

    auto* addBtn = createOpButton("+", '+');
    auto* subBtn = createOpButton("-", '-');
    auto* mulBtn = createOpButton("*", '*');
    auto* divBtn = createOpButton("/", '/');

    operationRow->add_child(addBtn);
    operationRow->add_child(subBtn);
    operationRow->add_child(mulBtn);
    operationRow->add_child(divBtn);

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}
