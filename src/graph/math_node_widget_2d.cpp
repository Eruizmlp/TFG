#include "math_node_widget_2d.h"
#include "framework/input.h"

using namespace GraphSystem;

MathNodeWidget2D::MathNodeWidget2D(const std::string& nodeType,
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
    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 140.0f };

    inspectPanel = new ui::XRPanel(
        "MathInspect_" + logic_node->getName(),
        pos,
        sz,
        0u,
        colors::BLUE
    );

    auto* mathNode = static_cast<MathNode*>(logic_node);

    float currentY = 20.0f;

    // Type selector (FLOAT, VEC3)
    auto* typeRow = new ui::HContainer2D("TypeRow", { 8, currentY });
    typeRow->item_margin = { 4, 0 };
    inspectPanel->add_child(typeRow);

    ui::sButtonDescription bdesc;
    bdesc.size = { 40, 24 };
    bdesc.color = colors::WHITE;

    floatBtn = new ui::Button2D("MathTypeFloat_" + logic_node->getName(), bdesc);
    floatBtn->add_child(new ui::Text2D("Float", { 0, 0 }));

    vec3Btn = new ui::Button2D("MathTypeVec3_" + logic_node->getName(), bdesc);
    vec3Btn->add_child(new ui::Text2D("Vec3", { 0, 0 }));

    typeRow->add_child(floatBtn);
    typeRow->add_child(vec3Btn);

    Node::bind(floatBtn->get_name(), FuncVoid([this, mathNode](const std::string&, void*) {
        mathNode->setMode(MathMode::FLOAT);
        }));

    Node::bind(vec3Btn->get_name(), FuncVoid([this, mathNode](const std::string&, void*) {
        mathNode->setMode(MathMode::VEC3);
        }));

    currentY += 40.0f;

    // Operation selector (+ - * /)
    auto* opRow = new ui::HContainer2D("OpRow", { 8, currentY });
    opRow->item_margin = { 4, 0 };
    inspectPanel->add_child(opRow);

    addBtn = new ui::Button2D("MathOpAdd_" + logic_node->getName(), bdesc);
    addBtn->add_child(new ui::Text2D("+", { 0, 0 }));

    subBtn = new ui::Button2D("MathOpSub_" + logic_node->getName(), bdesc);
    subBtn->add_child(new ui::Text2D("-", { 0, 0 }));

    mulBtn = new ui::Button2D("MathOpMul_" + logic_node->getName(), bdesc);
    mulBtn->add_child(new ui::Text2D("*", { 0, 0 }));

    divBtn = new ui::Button2D("MathOpDiv_" + logic_node->getName(), bdesc);
    divBtn->add_child(new ui::Text2D("/", { 0, 0 }));

    opRow->add_child(addBtn);
    opRow->add_child(subBtn);
    opRow->add_child(mulBtn);
    opRow->add_child(divBtn);

    Node::bind(addBtn->get_name(), FuncVoid([this, mathNode](const std::string&, void*) {
        mathNode->setOperation('+');
        }));

    Node::bind(subBtn->get_name(), FuncVoid([this, mathNode](const std::string&, void*) {
        mathNode->setOperation('-');
        }));

    Node::bind(mulBtn->get_name(), FuncVoid([this, mathNode](const std::string&, void*) {
        mathNode->setOperation('*');
        }));

    Node::bind(divBtn->get_name(), FuncVoid([this, mathNode](const std::string&, void*) {
        mathNode->setOperation('/');
        }));

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}

void MathNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
}
