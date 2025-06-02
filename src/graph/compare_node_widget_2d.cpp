#include "compare_node_widget_2d.h"
#include "framework/input.h"

namespace GraphSystem {

    CompareNodeWidget2D::CompareNodeWidget2D(const std::string& nodeType,
        CompareNode* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(nodeType, node, editor, worldPos), compareNode(node)
    {
    }

    void CompareNodeWidget2D::toggleInspector(sInputData data) {
        if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            if (!inspectPanel) initInspector();
            inspectorVisible = !inspectorVisible;
            inspectPanel->set_visibility(inspectorVisible, true);
        }
    }

    void CompareNodeWidget2D::updateInspector() {
        if (!inspectPanel || !inspectorVisible) return;
        inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    }

    void CompareNodeWidget2D::update(float delta_time) {
        NodeWidget2D::update(delta_time);
        updateInspector();
        if (inspectPanel && inspectorVisible)
            inspectPanel->update(delta_time);
    }

    void CompareNodeWidget2D::initInspector() {
        glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
        glm::vec2 sz = { 180, 120 };

        inspectPanel = new ui::XRPanel("CompareInspect_" + logic_node->getName(), pos, sz, 0u, colors::PURPLE);

        auto* container = new ui::VContainer2D("CompareBtns", { 8, 8 }, 0u, colors::GRAY);
        container->item_margin = { 4, 4 };
        inspectPanel->add_child(container);

        ui::sButtonDescription desc;
        desc.size = { 48, 24 };
        desc.color = colors::WHITE;

        using Op = CompareOp;
        std::vector<std::pair<std::string, Op>> ops = {
            { "==", Op::EQUAL },
            { "!=", Op::NOT_EQUAL },
            { "<",  Op::LESS },
            { ">",  Op::GREATER },
            { "<=", Op::LESS_EQUAL },
            { ">=", Op::GREATER_EQUAL }
        };

        for (auto& [label, op] : ops) {
            auto* btn = new ui::Button2D("CmpBtn_" + label, desc);
            btn->add_child(new ui::Text2D(label, { 0, 0 }));
            container->add_child(btn);

            Node::bind(btn->get_name(), FuncVoid([this, op](const std::string&, void*) {
                if (compareNode) compareNode->setOperation(op);
                }));
        }

        add_child(inspectPanel);
        inspectPanel->set_visibility(false, true);
    }

}
