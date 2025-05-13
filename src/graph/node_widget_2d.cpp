#include "node_widget_2d.h"
#include "port_button_2d.h"
#include "framework/input.h"
#include "framework/ui/io.h"
#include "framework/colors.h"
#include <GLFW/glfw3.h>

using namespace GraphSystem;
using namespace ui;

glm::vec2 NodeWidget2D::computeSize(GraphNode* node) {
    constexpr float W = 200.0f, ROW = 20.0f, M = 6.0f;
    size_t n = node->getOutputs().size() + node->getInputs().size();
    float  h = M + (3 + float(n)) * (ROW + M);
    return { W, h };
}

NodeWidget2D::NodeWidget2D(GraphNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : Node2D("Widget2D_" + node->getName(),
        { worldPos.x, worldPos.y },
        computeSize(node),
        ui::CREATE_3D),
    logic_node(node),
    graphEditor(editor)
{

    background = new XRPanel(
        "bg_" + node->getName(),
        { 0, 0 },
        get_size(),
        0u,
        colors::GRAY
    );
    add_child(background);

    // — root container —
    rootContainer = new VContainer2D(
        "NodeRoot_" + node->getName(),
        { 0, 0 },
        0u,
        colors::BLUE
    );

    rootContainer->padding = { 8, 8 };
    rootContainer->item_margin = { 4, 4 };
    rootContainer->set_fixed_size(get_size());
    background->add_child(rootContainer);

    float titleSize = 20.0f;
    auto* title = new Text2D(
        node->getName(),
        { 8, 8 },
        titleSize,
        SKIP_TEXT_RECT
    );
    title->set_color(colors::WHITE);
    rootContainer->add_child(title);

    // — OUTPUT ports —
    for (auto* outP : logic_node->getOutputs()) {
        auto* row = new HContainer2D(
            "RowOut_" + outP->getName(),
            { 0, 0 },
            0u,
            colors::GRAY
        );
        row->padding = { 4, 2 };
        row->item_margin = { 8, 0 };
        rootContainer->add_child(row);

        auto* lbl = new Text2D(
            outP->getName(),
            { 0, 0 },
            18.0f,
            SKIP_TEXT_RECT
        );
        lbl->set_color(colors::GREEN);
        row->add_child(lbl);

        sButtonDescription desc;
        desc.size = { 16, 16 };
        desc.color = colors::GREEN;
        auto* btn = new PortButton2D(
            "outBtn_" + node->getName() + "_" + outP->getName(),
            desc,
            logic_node,
            graphEditor,
            outP->getName(),
            true
        );
        row->add_child(btn);
    }

    // — INPUT ports —
    for (auto* inP : logic_node->getInputs()) {
        auto* row = new HContainer2D(
            "RowIn_" + inP->getName(),
            { 0, 0 },
            0u,
            colors::GRAY
        );
        row->padding = { 4, 2 };
        row->item_margin = { 8, 0 };
        rootContainer->add_child(row);

        sButtonDescription desc;
        desc.size = { 16, 16 };
        desc.color = colors::RED;
        auto* btn = new PortButton2D(
            "inBtn_" + node->getName() + "_" + inP->getName(),
            desc,
            logic_node,
            graphEditor,
            inP->getName(),
            false
        );
        row->add_child(btn);

        auto* lbl = new Text2D(
            inP->getName(),
            { 0, 0 },
            18.0f,
            SKIP_TEXT_RECT
        );
        lbl->set_color(colors::RED);
        row->add_child(lbl);
    }
}


sInputData NodeWidget2D::get_input_data(bool ignore_focus)
{
    return background->get_input_data(ignore_focus);
}

bool NodeWidget2D::on_input(sInputData data)
{
    if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        toggleInspector(data);
        return true;
    }
    
    return background->on_input(data);
}


void NodeWidget2D::update(float delta_time)
{

    auto d = get_input_data(false);
    if (d.was_pressed) {
        dragging = true;
        dragOffset = ::Input::get_mouse_position() - get_translation();
    }
    if (dragging && d.is_pressed) {
        set_position(::Input::get_mouse_position() - dragOffset);
    }
    if (d.was_released) {
        dragging = false;
    }

    updateInspector();

    background->update(delta_time);
}



RotateNodeWidget2D::RotateNodeWidget2D(RotateNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(node, editor, worldPos)
{
}

void RotateNodeWidget2D::toggleInspector(sInputData data) {
    
    if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void RotateNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    angleSlider->set_value(static_cast<RotateNode*>(logic_node)->getRotationAngle());
}

void RotateNodeWidget2D::update(float delta_time) {
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


void RotateNodeWidget2D::initInspector() {

    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 100.0f };

    inspectPanel = new XRPanel(
        "RotateInspect_" + logic_node->getName(),
        pos,
        sz,
        0u,
        colors::GRAY
    );

    auto* axisRow = new HContainer2D("AxisRow", { 8, sz.y - 30 }, 0u, colors::GRAY);

    axisRow->item_margin = { 4,0 };
    inspectPanel->add_child(axisRow);

    sButtonDescription bdesc;
    bdesc.size = { 24,24 };
    bdesc.color = colors::WHITE;

    axisXBtn = new Button2D("RotateAxisX_" + logic_node->getName(), bdesc);
    axisXBtn->add_child(new Text2D("X", { 0,0 }));
    axisYBtn = new Button2D("RotateAxisY_" + logic_node->getName(), bdesc);
    axisYBtn->add_child(new Text2D("Y", { 0,0 }));
    axisZBtn = new Button2D("RotateAxisZ_" + logic_node->getName(), bdesc);
    axisZBtn->add_child(new Text2D("Z", { 0,0 }));

    axisRow->add_child(axisXBtn);
    axisRow->add_child(axisYBtn);
    axisRow->add_child(axisZBtn);

    Node::bind(axisXBtn->get_name(), FuncVoid([this](const std::string&, void*) {

        static_cast<RotateNode*>(logic_node)->setRotationAxis({ 1,0,0 });
        }));


    Node::bind(axisYBtn->get_name(), FuncVoid([this](const std::string&, void*) {

        static_cast<RotateNode*>(logic_node)->setRotationAxis({ 0,1,0 });
        }));

    Node::bind(axisZBtn->get_name(), FuncVoid([this](const std::string&, void*) {
        static_cast<RotateNode*>(logic_node)->setRotationAxis({ 0,0,1 });
        }));

    // Angle slider
    sSliderDescription sd;
    sd.mode = HORIZONTAL;
    sd.position = { 8, sz.y - 60 };
    sd.size = { 80, 20 };
    sd.fvalue = static_cast<RotateNode*>(logic_node)->getRotationAngle();
    sd.fvalue_min = 0.0f;
    sd.fvalue_max = 360.0f;
    sd.precision = 1;

    angleSlider = new FloatSlider2D("AngleSlider_" + logic_node->getName(), sd);
    inspectPanel->add_child(angleSlider);

    Node::bind(angleSlider->get_name(), FuncFloat([this](const std::string&, float v) {
        static_cast<RotateNode*>(logic_node)->setRotationAngle(v);
        }));
    add_child(inspectPanel);

    inspectPanel->set_visibility(false, true);

}


// -------------------------- TranslateNodeWidget2D --------------------------

TranslateNodeWidget2D::TranslateNodeWidget2D(TranslateNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(node, editor, worldPos)
{
}

void TranslateNodeWidget2D::toggleInspector(sInputData data) {
    if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void TranslateNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    auto offs = static_cast<TranslateNode*>(logic_node)->getOffset();
    xSlider->set_value(offs.x);
    ySlider->set_value(offs.y);
    zSlider->set_value(offs.z);
}

void TranslateNodeWidget2D::update(float delta_time) {
    sInputData d = get_input_data(false);
    if (d.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }

    NodeWidget2D::update(delta_time);
    if (inspectPanel && inspectorVisible) inspectPanel->update(delta_time);
}

void TranslateNodeWidget2D::initInspector() {
    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 200.0f, 120.0f };
    inspectPanel = new XRPanel("TransInspect_" + logic_node->getName(), pos, sz, 0u, colors::GRAY);

    // X slider
    sSliderDescription sdX;
    sdX.mode = HORIZONTAL;
    sdX.position = { 8, sz.y - 30 };
    sdX.size = { 160, 20 };
    sdX.fvalue = static_cast<TranslateNode*>(logic_node)->getOffset().x;
    sdX.fvalue_min = -10.0f;
    sdX.fvalue_max = 10.0f;
    sdX.precision = 2;
    xSlider = new FloatSlider2D("TransX_" + logic_node->getName(), sdX);
    inspectPanel->add_child(xSlider);
    Node::bind(xSlider->get_name(), FuncFloat([this](const std::string&, float v) {
        auto o = static_cast<TranslateNode*>(logic_node)->getOffset();
        o.x = v;
        static_cast<TranslateNode*>(logic_node)->setOffset(o);
        }));

    // Y slider
    sSliderDescription sdY = sdX;
    sdY.position = { 8, sz.y - 60 };
    sdY.fvalue = static_cast<TranslateNode*>(logic_node)->getOffset().y;
    ySlider = new FloatSlider2D("TransY_" + logic_node->getName(), sdY);
    inspectPanel->add_child(ySlider);
    Node::bind(ySlider->get_name(), FuncFloat([this](const std::string&, float v) {
        auto o = static_cast<TranslateNode*>(logic_node)->getOffset();
        o.y = v;
        static_cast<TranslateNode*>(logic_node)->setOffset(o);
        }));

    // Z slider
    sSliderDescription sdZ = sdX;
    sdZ.position = { 8, sz.y - 90 };
    sdZ.fvalue = static_cast<TranslateNode*>(logic_node)->getOffset().z;
    zSlider = new FloatSlider2D("TransZ_" + logic_node->getName(), sdZ);
    inspectPanel->add_child(zSlider);
    Node::bind(zSlider->get_name(), FuncFloat([this](const std::string&, float v) {
        auto o = static_cast<TranslateNode*>(logic_node)->getOffset();
        o.z = v;
        static_cast<TranslateNode*>(logic_node)->setOffset(o);
        }));

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}

// -------------------------- ScaleNodeWidget2D --------------------------

ScaleNodeWidget2D::ScaleNodeWidget2D(ScaleNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(node, editor, worldPos)
{
}

void ScaleNodeWidget2D::toggleInspector(sInputData data) {
    if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void ScaleNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    factorSlider->set_value(static_cast<ScaleNode*>(logic_node)->getScaleFactor());
}

void ScaleNodeWidget2D::update(float delta_time) {
    sInputData d = get_input_data(false);
    if (d.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }

    NodeWidget2D::update(delta_time);
    if (inspectPanel && inspectorVisible) inspectPanel->update(delta_time);
}

void ScaleNodeWidget2D::initInspector() {
    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 80.0f };
    inspectPanel = new XRPanel("ScaleInspect_" + logic_node->getName(), pos, sz, 0u, colors::GRAY);

    sSliderDescription sd;
    sd.mode = HORIZONTAL;
    sd.position = { 8, sz.y - 60 };
    sd.size = { 80, 20 };
    sd.fvalue = static_cast<ScaleNode*>(logic_node)->getScaleFactor();
    sd.fvalue_min = 0.1f;
    sd.fvalue_max = 5.0f;
    sd.precision = 2;
    factorSlider = new FloatSlider2D("ScaleFac_" + logic_node->getName(), sd);
    inspectPanel->add_child(factorSlider);
    Node::bind(factorSlider->get_name(), FuncFloat([this](const std::string&, float v) {
        static_cast<ScaleNode*>(logic_node)->setScaleFactor(v);
        }));

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}




