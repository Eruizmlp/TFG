#include "entity_node_widget_2d.h"
#include "graph/context_menu.h"
#include "engine/sample_engine.h"
#include "engine/scene.h"
#include "framework/input.h"
#include "framework/nodes/text.h"
#include "framework/colors.h"
#include <iostream>

namespace GraphSystem {

    EntityNodeWidget2D::EntityNodeWidget2D(const std::string& nodeType,
        EntityNode3D* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(nodeType, node, editor, worldPos),
        entityNode(node) {
    }

    void EntityNodeWidget2D::toggleInspector(sInputData data) {
        if (!data.is_hovered || !::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT))
            return;

        auto* eng = SampleEngine::get_sample_instance();
        Node2D* container = eng ? eng->get_graph_container() : nullptr;
        if (!container)
            return;

        std::vector<ui::sContextMenuOption> options;

        Scene* scene = eng->get_main_scene();
        for (Node* node : scene->get_nodes()) {
            if (auto* mesh = dynamic_cast<MeshInstance3D*>(node)) {
                ui::sContextMenuOption option;
                option.name = std::string(mesh->get_name());

                option.event = FuncInt([this, mesh](const std::string&, int) {
                    setEntity(mesh);
                    });
                options.push_back(option);
            }
        }

        if (options.empty()) return;

        glm::vec2 menuPos = get_translation() + glm::vec2(get_size().x + 20.f, 0.f);
        glm::vec3 worldPos = { menuPos.x, menuPos.y, 0.f };

        auto* contextMenu = new ui::ContextMenu(menuPos, worldPos, options);
        container->add_child(contextMenu);  
    }

    void EntityNodeWidget2D::updateInspector() {
        // Not used
    }

    void EntityNodeWidget2D::update(float delta_time) {
        NodeWidget2D::update(delta_time);
        sInputData d = get_input_data(false);
        if (d.is_hovered && ::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            toggleInspector(d);
        }
    }

    void EntityNodeWidget2D::setEntity(MeshInstance3D* e) {
        entity = e;
        if (entityNode) {
            entityNode->setEntity(e);
        }
    }
}
