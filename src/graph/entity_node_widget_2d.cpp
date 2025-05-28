// ContextMenu integration in EntityNodeWidget2D
// Assuming your context_menu.cpp/h is working correctly and already in use

#include "entity_node_widget_2d.h"
#include "graph/context_menu.h"
#include "engine/sample_engine.h"
#include "engine/scene.h"
#include "framework/input.h"
#include "framework/nodes/text.h"
#include "framework/colors.h"
#include <iostream>

namespace GraphSystem {

    EntityNodeWidget2D::EntityNodeWidget2D(EntityNode3D* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(node, editor, worldPos),
        entityNode(node) {
    }

    void EntityNodeWidget2D::toggleInspector(sInputData data) {
        if (!data.is_hovered || !::Input::was_mouse_pressed(GLFW_MOUSE_BUTTON_RIGHT)) return;

        std::vector<ui::sContextMenuOption> options;

        auto* eng = SampleEngine::get_sample_instance();
        if (!eng || !eng->get_main_scene()) return;

        for (Node* node : eng->get_main_scene()->get_nodes()) {
            auto* mesh = dynamic_cast<MeshInstance3D*>(node);
            if (!mesh) continue;

            ui::sContextMenuOption opt;
            opt.name = mesh->get_name();
            opt.event = FuncInt([this, mesh](const std::string&, int) {
                entityNode->setEntity(mesh);
                setEntity(mesh);
                });

            options.push_back(opt);
        }

        glm::vec2 menuPos = get_translation() + glm::vec2(get_size().x + 20.f, 0.f);
        glm::vec3 worldPos = { menuPos.x, menuPos.y, 0.f };

        auto* contextMenu = new ui::ContextMenu(menuPos, worldPos, options);
        eng->get_main_scene()->add_node(contextMenu);
    }

    void EntityNodeWidget2D::updateInspector() {
        // no-op, handled by context menu
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
