#include "set_color_node_widget_2d.h"
#include "framework/input.h"
#include "framework/ui/ui_utils.h" 
#include "framework/colors.h"

namespace GraphSystem {

    SetColorNodeWidget2D::SetColorNodeWidget2D(const std::string& nodeType,
        SetColorNode* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(nodeType, node, editor, worldPos), setColorNode(node)
    {
    }

    void SetColorNodeWidget2D::toggleInspector(sInputData data) {
        if (data.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
            if (!inspectPanel) initInspector();
            inspectorVisible = !inspectorVisible;
            if (inspectPanel) inspectPanel->set_visibility(inspectorVisible, true);
        }
    }

    void SetColorNodeWidget2D::update(float delta_time) {
        NodeWidget2D::update(delta_time);
        updateInspector();
        if (inspectPanel && inspectorVisible)
            inspectPanel->update(delta_time);
    }

    void SetColorNodeWidget2D::updateInspector() {
        if (!inspectPanel || !inspectorVisible) return;

        inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });

      
        float r = setColorNode->getInput("R")->getFloat();
        float g = setColorNode->getInput("G")->getFloat();
        float b = setColorNode->getInput("B")->getFloat();
        float a = setColorNode->getInput("A")->getFloat();

        rSlider->set_value(r);
        gSlider->set_value(g);
        bSlider->set_value(b);
        aSlider->set_value(a);

        rValueLabel->set_text(formatFloat(r, 2));
        gValueLabel->set_text(formatFloat(g, 2));
        bValueLabel->set_text(formatFloat(b, 2));
        aValueLabel->set_text(formatFloat(a, 2));
    }

    void SetColorNodeWidget2D::initInspector() {
        glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
        glm::vec2 sz = { 220, 180 };

        inspectPanel = new ui::XRPanel("SetColorInspect_" + logic_node->getName(), pos, sz, 0u, colors::PURPLE);

        auto* container = new ui::VContainer2D("ColorSliders", { 8, 8 }, 0u, colors::GRAY);
        container->item_margin = { 2, 8 };
        inspectPanel->add_child(container);

    
        auto createSliderRow = [&](const std::string& label, float initialValue, ui::Text2D*& valueLabelPtr) -> ui::FloatSlider2D* {
            auto* row = new ui::HContainer2D(label + "Row", { 0,0 });
            container->add_child(row);
            row->add_child(new ui::Text2D(label, { 0,0 }, 18.0f, ui::SKIP_TEXT_RECT));
            ui::sSliderDescription sd;
            sd.size = { 100, 20 };
            sd.fvalue = initialValue;
            sd.fvalue_min = 0.0f;
            sd.fvalue_max = 1.0f;
            auto* slider = new ui::FloatSlider2D("Slider_" + label, sd);
            row->add_child(slider);
            valueLabelPtr = new ui::Text2D(formatFloat(initialValue, 2), { 0,0 }, 18.0f, ui::SKIP_TEXT_RECT);
            row->add_child(valueLabelPtr);
            return slider;
            };

        rSlider = createSliderRow("R:", setColorNode->getInput("R")->getFloat(), rValueLabel);
        gSlider = createSliderRow("G:", setColorNode->getInput("G")->getFloat(), gValueLabel);
        bSlider = createSliderRow("B:", setColorNode->getInput("B")->getFloat(), bValueLabel);
        aSlider = createSliderRow("A:", setColorNode->getInput("A")->getFloat(), aValueLabel);

        Node::bind(rSlider->get_name(), FuncFloat([this](const std::string&, float v) {
            setColorNode->setDefaultR(v);
            rValueLabel->set_text(formatFloat(v, 2));
            }));

        Node::bind(gSlider->get_name(), FuncFloat([this](const std::string&, float v) {
            setColorNode->setDefaultG(v);
            gValueLabel->set_text(formatFloat(v, 2));
            }));

        Node::bind(bSlider->get_name(), FuncFloat([this](const std::string&, float v) {
            setColorNode->setDefaultB(v);
            bValueLabel->set_text(formatFloat(v, 2));
            }));

        Node::bind(aSlider->get_name(), FuncFloat([this](const std::string&, float v) {
            setColorNode->setDefaultA(v);
            aValueLabel->set_text(formatFloat(v, 2));
            }));

        add_child(inspectPanel);
        inspectPanel->set_visibility(false, true);
    }
}
