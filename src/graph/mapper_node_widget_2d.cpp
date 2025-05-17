#include "mapper_node_widget_2d.h"
#include "framework/input.h"

namespace GraphSystem {

    MapperNodeWidget2D::MapperNodeWidget2D(MapperNode* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(node, editor, worldPos), mapperNode(node)
    {
    }

    void MapperNodeWidget2D::toggleInspector(sInputData data) {
        if (data.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            if (!inspectPanel) initInspector();
            inspectorVisible = !inspectorVisible;
            inspectPanel->set_visibility(inspectorVisible, true);
        }
    }

    void MapperNodeWidget2D::updateInspector() {
        if (!inspectPanel || !inspectorVisible) return;
        inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    }

    void MapperNodeWidget2D::update(float delta_time) {
        NodeWidget2D::update(delta_time);
        updateInspector();
        if (inspectPanel && inspectorVisible)
            inspectPanel->update(delta_time);
    }

    void MapperNodeWidget2D::initInspector() {
        glm::vec2 pos = get_translation() + glm::vec2(get_size().x + 10.0f, 0.0f);
        glm::vec2 sz = { 180, 80 };

        inspectPanel = new ui::XRPanel("MapperInspector_" + logic_node->getName(), pos, sz, 0u, colors::GRAY);

        auto* container = new ui::HContainer2D("ButtonRow", { 8, 8 }, 0u, colors::GRAY);
        container->item_margin = { 4, 4 };
        inspectPanel->add_child(container);

        ui::sButtonDescription desc;
        desc.size = { 32, 24 };
        desc.color = colors::WHITE;

        std::vector<std::pair<std::string, int>> buttons = {
            { "A", GLFW_KEY_A },
            { "B", GLFW_KEY_B },
            { "X", GLFW_KEY_X },
            { "Y", GLFW_KEY_Y }
        };

        for (auto& [label, keyCode] : buttons) {
            auto* btn = new ui::Button2D("KeyBtn_" + label, desc);
            btn->add_child(new ui::Text2D(label, { 0, 0 }));
            container->add_child(btn);

            Node::bind(btn->get_name(), FuncVoid([this, keyCode](const std::string&, void*) {
                if (mapperNode) mapperNode->setMappedButton(keyCode);
                }));
        }

        add_child(inspectPanel);
        inspectPanel->set_visibility(false, true);
    }

}
