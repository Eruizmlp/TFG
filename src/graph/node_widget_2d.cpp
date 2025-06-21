#include "node_widget_2d.h"
#include "port_button_2d.h"
#include "framework/input.h"
#include "framework/ui/io.h"
#include "framework/colors.h"
#include <GLFW/glfw3.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "graphics/renderer.h"


using namespace GraphSystem;
using namespace ui;

glm::vec4 NodeWidget2D::getColorForCategory(GraphSystem::NodeCategory cat) {
    switch (cat) {
    case NodeCategory::DATA:        return colors::CYAN * 0.5f;
    case NodeCategory::FLOW:        return colors::YELLOW * 0.5f;
    case NodeCategory::TRANSFORM:   return colors::GREEN * 0.5f;
    case NodeCategory::LOGIC:       return colors::PURPLE * 0.5f;
    case NodeCategory::INTERACTION: return colors::RUST * 0.5f;
    default:                        return colors::GRAY * 0.5f;
    }
}


inline std::string formatFloat(float value, int precision = 2) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}


glm::vec2 NodeWidget2D::computeSize(GraphNode* node) {
    constexpr float W = 200.0f, ROW = 20.0f, M = 6.0f;
    size_t n = node->getOutputs().size() + node->getInputs().size();
    float  h = M + (3 + float(n)) * (ROW + M);
    return { W, h };
}

NodeWidget2D::NodeWidget2D(const std::string& nodeType,
    GraphNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : Node2D("Widget2D_" + node->getName(),
        { worldPos.x, worldPos.y },
        computeSize(node),
        ui::CREATE_3D),
    logic_node(node),
    graphEditor(editor),
    graphNodeType(nodeType)
{
    background = new XRPanel(
        "bg_" + node->getName(),
        { 0, 0 },
        get_size(),
        0u,
        getColorForCategory(node->getCategory())
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
    titleLabel = new Text2D(
        node->getName(),
        { 8, 8 },
        titleSize,
        SKIP_TEXT_RECT
    );
    titleLabel->set_color(colors::WHITE);
    rootContainer->add_child(titleLabel);

    rebuildWidgetUI();
}


sInputData NodeWidget2D::get_input_data(bool ignore_focus)
{
    return background->get_input_data(ignore_focus);
}

bool NodeWidget2D::on_input(sInputData data) {
    if (data.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
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


void NodeWidget2D::updateTitleFromLogicNode() {
    if (logic_node && titleLabel) {
        titleLabel->set_text(logic_node->getName());
    }
}
void NodeWidget2D::rebuildWidgetUI() {
    if (!rootContainer || !logic_node) return;

    // 1. Crear una lista de hijos a borrar para evitar problemas al modificar la lista mientras se itera.
    //    No incluimos el titleLabel en esta lista para preservarlo.
    std::vector<Node*> children_to_delete;
    for (auto* child : rootContainer->get_children()) {
        if (child != titleLabel) {
            children_to_delete.push_back(child);
        }
    }

    // 2. Borrar los hijos seleccionados usando el método correcto del padre.
    for (auto* child_node : children_to_delete) {
        // FIX: Realizar un dynamic_cast a Node2D* para llamar a la sobrecarga correcta de remove_child,
        // que es vital para que los contenedores de UI actualicen su layout.
        if (Node2D* node2d_child = dynamic_cast<Node2D*>(child_node)) {
            rootContainer->remove_child(node2d_child);
        }
        delete child_node; // Liberar la memoria del nodo hijo.
    }

    // 3. Vuelve a crear la UI para los pines con los punteros ahora válidos.
    for (auto* outP : logic_node->getOutputs()) {
        auto* row = new ui::HContainer2D("RowOut_" + logic_node->getName() + "_" + outP->getName(), { 0, 0 });
        row->padding = { 4, 2 }; row->item_margin = { 8, 0 };
        rootContainer->add_child(row);

        auto* lbl = new ui::Text2D(outP->getName(), { 0, 0 }, 18.0f, SKIP_TEXT_RECT);
        lbl->set_color(colors::GREEN);
        row->add_child(lbl);

        ui::sButtonDescription desc; desc.size = { 16, 16 }; desc.color = colors::GREEN;
        auto* btn = new PortButton2D("outBtn_" + logic_node->getName() + "_" + outP->getName(), desc, logic_node, graphEditor, outP->getName(), true);
        row->add_child(btn);
    }

    for (auto* inP : logic_node->getInputs()) {
        auto* row = new ui::HContainer2D("RowIn_" + logic_node->getName() + "_" + inP->getName(), { 0, 0 });
        row->padding = { 4, 2 }; row->item_margin = { 8, 0 };
        rootContainer->add_child(row);

        ui::sButtonDescription desc; desc.size = { 16, 16 }; desc.color = colors::RED;
        auto* btn = new PortButton2D("inBtn_" + logic_node->getName() + "_" + inP->getName(), desc, logic_node, graphEditor, inP->getName(), false);
        row->add_child(btn);

        auto* lbl = new ui::Text2D(inP->getName(), { 0, 0 }, 18.0f, SKIP_TEXT_RECT);
        lbl->set_color(colors::RED);
        row->add_child(lbl);
    }
}

void NodeWidget2D::serialize(std::ofstream& binary_scene_file)
{
    uint64_t node_type_size = graphNodeType.size();
    binary_scene_file.write(reinterpret_cast<const char*>(&node_type_size), sizeof(uint64_t));
    binary_scene_file.write(graphNodeType.c_str(), node_type_size);
    logic_node->serialize(binary_scene_file);
    glm::vec2 current_pos = get_translation();
    binary_scene_file.write(reinterpret_cast<const char*>(&current_pos), sizeof(glm::vec2));
}

void NodeWidget2D::parse(std::ifstream& binary_scene_file)
{
    logic_node->parse(binary_scene_file);
    glm::vec2 current_pos;
    binary_scene_file.read(reinterpret_cast<char*>(&current_pos), sizeof(glm::vec2));
    set_position(current_pos);
}


void RotateNodeWidget2D::serialize(std::ofstream& binary_scene_file)
{
    NodeWidget2D::serialize(binary_scene_file);
    auto* rotateNode = static_cast<RotateNode*>(logic_node);
    float angle = rotateNode->getRotationAngle();
    const glm::vec3& axis = rotateNode->getRotationAxis();
    binary_scene_file.write(reinterpret_cast<const char*>(&angle), sizeof(float));
    binary_scene_file.write(reinterpret_cast<const char*>(&axis), sizeof(glm::vec3));
}

void RotateNodeWidget2D::parse(std::ifstream& binary_scene_file)
{
    NodeWidget2D::parse(binary_scene_file);
    auto* rotateNode = static_cast<RotateNode*>(logic_node);
    float angle;
    glm::vec3 axis;
    binary_scene_file.read(reinterpret_cast<char*>(&angle), sizeof(float));
    binary_scene_file.read(reinterpret_cast<char*>(&axis), sizeof(glm::vec3));
    rotateNode->setRotationAngle(angle);
    rotateNode->setRotationAxis(axis);

    if (Input* angleInput = rotateNode->getInput("Angle")) {
        angleInput->setData(VariableValue(angle));
    }
}


RotateNodeWidget2D::RotateNodeWidget2D(const std::string& nodeType,
    RotateNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(nodeType, node, editor, worldPos)
{

}



void RotateNodeWidget2D::toggleInspector(sInputData data) {
    
    if (data.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void RotateNodeWidget2D::update(float delta_time) {
    sInputData d = get_input_data(false);
    if (d.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }

    NodeWidget2D::update(delta_time);

    
    if (inspectPanel && inspectorVisible) {
        inspectPanel->update(delta_time);
    }
}

void RotateNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });

    auto* rotateNode = static_cast<RotateNode*>(logic_node);

    float rawAngle = rotateNode->getRotationAngle();

    float displayAngle = std::fmod(rawAngle, 360.0f);
    if (displayAngle < 0) {
        displayAngle += 360.0f;
    }

    angleSlider->set_value(displayAngle);
    
    
    angleValueLabel->set_text(formatFloat(displayAngle, 1));
}

void RotateNodeWidget2D::initInspector() {
    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 120.0f };

    inspectPanel = new XRPanel(
        "RotateInspect_" + logic_node->getName(),
        pos,
        sz,
        0u,
        colors::GREEN
    );

    auto* rotateNode = static_cast<RotateNode*>(logic_node);

    // Angle slider label
    float currentY = 20.0f;
    auto* label = new Text2D("Angle:", { 8, currentY });
    inspectPanel->add_child(label);
    currentY += 20.0f;

    // Angle slider
    sSliderDescription sd;
    sd.mode = HORIZONTAL;
    sd.position = { 8, currentY };
    sd.size = { 80, 20 };
    sd.fvalue = rotateNode->getRotationAngle();
    sd.fvalue_min = 0.0f;
    sd.fvalue_max = 360.0f; 
    sd.precision = 1;

    angleSlider = new FloatSlider2D("AngleSlider_" + logic_node->getName(), sd);
    inspectPanel->add_child(angleSlider);

    // Value label (angle dynamic update)
    angleValueLabel = new Text2D(formatFloat(rotateNode->getRotationAngle(), 1), { 140, currentY });
    inspectPanel->add_child(angleValueLabel);

    Node::bind(angleSlider->get_name(), FuncFloat([this, rotateNode](const std::string&, float v) {

        v = fmod(fmod(v, 360.0f) + 360.0f, 360.0f);

        rotateNode->setRotationAngle(v); 

        if (rotateNode->getInput("Angle")) { 
            rotateNode->getInput("Angle")->setData(VariableValue(v)); 
        }


        if (angleValueLabel) {
            angleValueLabel->set_text(formatFloat(v, 1)); 
        }
        }));

    currentY += 30.0f;

    // Axis buttons
    auto* axisRow = new HContainer2D("AxisRow", { 8, currentY }, 0u, colors::GRAY);
    axisRow->item_margin = { 4, 0 };
    inspectPanel->add_child(axisRow);

    sButtonDescription bdesc;
    bdesc.size = { 24, 24 };
    bdesc.color = colors::WHITE;

    axisXBtn = new Button2D("RotateAxisX_" + logic_node->getName(), bdesc);
    axisXBtn->add_child(new Text2D("X", { 0, 0 }));

    axisYBtn = new Button2D("RotateAxisY_" + logic_node->getName(), bdesc);
    axisYBtn->add_child(new Text2D("Y", { 0, 0 }));

    axisZBtn = new Button2D("RotateAxisZ_" + logic_node->getName(), bdesc);
    axisZBtn->add_child(new Text2D("Z", { 0, 0 }));

    axisRow->add_child(axisXBtn);
    axisRow->add_child(axisYBtn);
    axisRow->add_child(axisZBtn);

    Node::bind(axisXBtn->get_name(), FuncVoid([this](const std::string&, void*) {
        static_cast<RotateNode*>(logic_node)->setRotationAxis({ 1, 0, 0 });
        }));

    Node::bind(axisYBtn->get_name(), FuncVoid([this](const std::string&, void*) {
        static_cast<RotateNode*>(logic_node)->setRotationAxis({ 0, 1, 0 });
        }));

    Node::bind(axisZBtn->get_name(), FuncVoid([this](const std::string&, void*) {
        static_cast<RotateNode*>(logic_node)->setRotationAxis({ 0, 0, 1 });
        }));

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}




// -------------------------- TranslateNodeWidget2D --------------------------

TranslateNodeWidget2D::TranslateNodeWidget2D(const std::string& nodeType,
    TranslateNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(nodeType, node, editor, worldPos)
{
}

void TranslateNodeWidget2D::toggleInspector(sInputData data) {
    if (data.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}

void TranslateNodeWidget2D::update(float delta_time) {
    sInputData d = get_input_data(false);
    if (d.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }

    NodeWidget2D::update(delta_time);
    if (inspectPanel && inspectorVisible) inspectPanel->update(delta_time);
}
void TranslateNodeWidget2D::initInspector() {
    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 250.0f };
    inspectPanel = new XRPanel("TransInspect_" + logic_node->getName(), pos, sz, 0u, colors::GREEN);

    float currentY = 20.0f;
    float margin = 10.0f;

    const float minPosition = -1000.0f;
    const float maxPosition = 1000.0f;
    const int precision = 2;

    auto createLabelAndSlider = [this, &currentY, margin, minPosition, maxPosition, precision](const std::string& labelText, const std::string& sliderName, float initialValue, Text2D*& valueLabelPtr, std::function<void(float)> callback) {
        auto* label = new Text2D(labelText, { 8, currentY });
        inspectPanel->add_child(label);
        currentY += 20.0f;

        sSliderDescription sd;
        sd.mode = HORIZONTAL;
        sd.position = { 8, currentY };
        sd.size = { 80, 20 };
        sd.fvalue = initialValue;
        sd.fvalue_min = minPosition;
        sd.fvalue_max = maxPosition;
        sd.precision = precision;

        auto* slider = new FloatSlider2D(sliderName, sd);
        inspectPanel->add_child(slider);

        valueLabelPtr = new Text2D(formatFloat(initialValue, precision), { 100, currentY });
        inspectPanel->add_child(valueLabelPtr);

        Node::bind(slider->get_name(), FuncFloat([callback, valueLabelPtr, precision](const std::string&, float v) {
            valueLabelPtr->set_text(formatFloat(v, precision));
            callback(v);
            }));

        currentY += 30.0f + margin;

        return slider;
        };

    auto offset = static_cast<TranslateNode*>(logic_node)->getOffset();

    xSlider = createLabelAndSlider("X:", "TransX_" + logic_node->getName(), offset.x, xValueLabel, [this](float v_comp) { 
        auto* node = static_cast<TranslateNode*>(logic_node);
        glm::vec3 currentFullOffset = node->getOffset(); 
        currentFullOffset.x = v_comp;
        node->setOffset(currentFullOffset); 
        if (node->getInput("Offset")) {
            node->getInput("Offset")->setData(VariableValue(currentFullOffset)); 
        }
        });

    xSlider = createLabelAndSlider("Y:", "TransY_" + logic_node->getName(), offset.y, xValueLabel, [this](float v_comp) { 
        auto* node = static_cast<TranslateNode*>(logic_node);
        glm::vec3 currentFullOffset = node->getOffset(); 
        currentFullOffset.y = v_comp;
        node->setOffset(currentFullOffset); 
        if (node->getInput("Offset")) {
            node->getInput("Offset")->setData(VariableValue(currentFullOffset)); 
        }
        });

    xSlider = createLabelAndSlider("Z:", "TransZ_" + logic_node->getName(), offset.z, xValueLabel, [this](float v_comp) { 
        auto* node = static_cast<TranslateNode*>(logic_node);
        glm::vec3 currentFullOffset = node->getOffset();
        currentFullOffset.z = v_comp;
        node->setOffset(currentFullOffset); 
        if (node->getInput("Offset")) {
            node->getInput("Offset")->setData(VariableValue(currentFullOffset)); 
        }
        });

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}

void TranslateNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });

    auto* translateNode = static_cast<TranslateNode*>(logic_node);
    auto* offsetInput = translateNode->getInput("Offset");

    glm::vec3 offset = translateNode->getOffset();
    if (offsetInput && offsetInput->hasData()) {
        offset = offsetInput->getVec3();
    }

    xSlider->set_value(offset.x);
    xValueLabel->set_text(formatFloat(offset.x, 2));

    ySlider->set_value(offset.y);
    yValueLabel->set_text(formatFloat(offset.y, 2));

    zSlider->set_value(offset.z);
    zValueLabel->set_text(formatFloat(offset.z, 2));
}


// -------------------------- ScaleNodeWidget2D --------------------------

ScaleNodeWidget2D::ScaleNodeWidget2D(const std::string& nodeType,
    ScaleNode* node,
    GraphEditor* editor,
    const glm::vec3& worldPos)
    : NodeWidget2D(nodeType, node, editor, worldPos)
{
}

void ScaleNodeWidget2D::toggleInspector(sInputData data) {
    if (data.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }
}


void ScaleNodeWidget2D::update(float delta_time) {
    sInputData d = get_input_data(false);
    if (d.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        inspectPanel->set_visibility(inspectorVisible, true);
    }

    NodeWidget2D::update(delta_time);
    if (inspectPanel && inspectorVisible) inspectPanel->update(delta_time);
}

void ScaleNodeWidget2D::initInspector() {
    glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
    glm::vec2 sz = { 180.0f, 100.0f };
    inspectPanel = new XRPanel("ScaleInspect_" + logic_node->getName(), pos, sz, 0u, colors::GREEN);

    auto* scaleNode = static_cast<ScaleNode*>(logic_node);

    float currentY = 20.0f;

    auto* label = new Text2D("Scale:", { 8, currentY });
    inspectPanel->add_child(label);
    currentY += 20.0f;

    sSliderDescription sd;
    sd.mode = HORIZONTAL;
    sd.position = { 8, currentY };
    sd.size = { 80, 20 };
    sd.fvalue = scaleNode->getScaleFactor();
    sd.fvalue_min = 0.1f;
    sd.fvalue_max = 10.0f;
    sd.precision = 2;

    factorSlider = new FloatSlider2D("ScaleFac_" + logic_node->getName(), sd);
    inspectPanel->add_child(factorSlider);

    scaleValueLabel = new Text2D(formatFloat(scaleNode->getScaleFactor(), 2), { 100, currentY });
    inspectPanel->add_child(scaleValueLabel);

    Node::bind(factorSlider->get_name(), FuncFloat([this, scaleNode](const std::string&, float v) {
        scaleNode->setScaleFactor(v); // Actualiza el miembro interno

        // Actualizar el Input "Factor" del nodo
        if (scaleNode->getInput("Factor")) { 
            scaleNode->getInput("Factor")->setData(VariableValue(v));
        }


        if (scaleValueLabel) { // Asegurarse que scaleValueLabel no es null
            scaleValueLabel->set_text(formatFloat(v, 2)); // 
        }
        }));

    add_child(inspectPanel);
    inspectPanel->set_visibility(false, true);
}

void ScaleNodeWidget2D::updateInspector() {
    if (!inspectPanel || !inspectorVisible) return;
    inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });

    auto* scaleNode = static_cast<ScaleNode*>(logic_node);
    auto* factorInput = scaleNode->getInput("Factor");

    float valueToDisplay = scaleNode->getScaleFactor();
    if (factorInput && factorInput->hasData()) {
        valueToDisplay = factorInput->getFloat();
    }

    factorSlider->set_value(valueToDisplay);
    scaleValueLabel->set_text(formatFloat(valueToDisplay, 2));
}




