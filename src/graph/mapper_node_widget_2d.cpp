#include "mapper_node_widget_2d.h"
#include "framework/input.h"
#include "mapper_node.h" 

namespace GraphSystem {

    MapperNodeWidget2D::MapperNodeWidget2D(const std::string& nodeType,
        MapperNode* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(nodeType, node, editor, worldPos), mapperNode(node)
    {
    }

    void MapperNodeWidget2D::toggleInspector(sInputData data) {
        if (!inspectPanel) initInspector();
        inspectorVisible = !inspectorVisible;
        if (inspectPanel) inspectPanel->set_visibility(inspectorVisible, true);
    }


    void MapperNodeWidget2D::updateInspector() {
        if (!inspectPanel || !inspectorVisible) return;
        inspectPanel->set_position({ get_size().x + 10.0f, 0.0f });
    }

    void MapperNodeWidget2D::update(float delta_time) {
        sInputData d = get_input_data(false);
        if (d.is_hovered && (::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT) || (::Input::was_trigger_pressed(HAND_LEFT)))) {
        
            toggleInspector(d);
        }

        NodeWidget2D::update(delta_time); 

        updateInspector();

        if (inspectPanel && inspectorVisible)
            inspectPanel->update(delta_time);
    }


    void MapperNodeWidget2D::initInspector() {
        glm::vec2 pos = glm::vec2(get_size().x + 10.0f, 0.0f);
        glm::vec2 sz = { 180, 80 };

        inspectPanel = new ui::XRPanel("MapperInspector_" + logic_node->getName(), pos, sz, 0u, colors::RUST);

        auto* container = new ui::HContainer2D("ButtonRow", { 8, 8 }, 0u, colors::GRAY);
        container->item_margin = { 4, 4 };
        inspectPanel->add_child(container);

        ui::sButtonDescription desc;
        desc.size = { 32, 24 };
        desc.color = colors::WHITE;

       
        std::vector<std::pair<std::string, MappedButton>> buttons = {
            { "A", MappedButton::A },
            { "B", MappedButton::B },
            { "X", MappedButton::X },
            { "Y", MappedButton::Y }
        };

        for (const auto& [label, buttonEnum] : buttons) {
            auto* btn = new ui::Button2D("KeyBtn_" + label, desc);
            btn->add_child(new ui::Text2D(label, { 0, 0 }));
            container->add_child(btn);
 
            Node::bind(btn->get_name(), FuncVoid([this, buttonEnum](const std::string&, void*) {
                if (mapperNode) {
                    mapperNode->setMappedButton(buttonEnum);
                }
                }));
        }

        add_child(inspectPanel);
        inspectPanel->set_visibility(false, true);
    }
}
