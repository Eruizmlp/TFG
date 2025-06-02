#include "trigonometric_node_widget_2d.h"
#include "framework/input.h"

namespace GraphSystem {

    TrigonometricNodeWidget2D::TrigonometricNodeWidget2D(const std::string& nodeType,
        TrigonometricNode* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(nodeType, node, editor, worldPos), trigNode(node)
    {
    }

    void TrigonometricNodeWidget2D::toggleInspector(sInputData data) {
        if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            if (!inspectPanel) initInspector();
            inspectorVisible = !inspectorVisible;
            inspectPanel->set_visibility(inspectorVisible, true);
        }
    }

    void TrigonometricNodeWidget2D::updateInspector() {
        if (!inspectPanel || !inspectorVisible) return;
        inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    }

    void TrigonometricNodeWidget2D::update(float delta_time) {
        NodeWidget2D::update(delta_time);
        updateInspector();
        if (inspectPanel && inspectorVisible)
            inspectPanel->update(delta_time);
    }

    void TrigonometricNodeWidget2D::initInspector() {
        glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
        glm::vec2 sz = { 160, 100 };

        inspectPanel = new ui::XRPanel("TrigInspector_" + logic_node->getName(), pos, sz, 0u, colors::CYAN);

        auto* container = new ui::VContainer2D("TrigFuncBtns", { 8, 8 }, 0u, colors::GRAY);
        container->item_margin = { 4, 4 };
        inspectPanel->add_child(container);

        ui::sButtonDescription desc;
        desc.size = { 40, 24 };
        desc.color = colors::WHITE;

        std::vector<std::pair<std::string, char>> funcs = {
            { "sin", 's' }, { "cos", 'c' }, { "tan", 't' }
        };

        for (auto& [label, opChar] : funcs) {
            auto* btn = new ui::Button2D("TrigBtn_" + label, desc);
            btn->add_child(new ui::Text2D(label, { 0, 0 }));
            container->add_child(btn);

            Node::bind(btn->get_name(), FuncVoid([this, opChar](const std::string&, void*) {
                if (trigNode) trigNode->setOperation(opChar);
                }));
        }

        add_child(inspectPanel);
        inspectPanel->set_visibility(false, true);
    }

}
