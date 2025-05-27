#include "entity_node_widget_2d.h"
#include "framework/nodes/mesh_instance_3d.h"
#include "engine/scene.h"

namespace GraphSystem {

    EntityNodeWidget2D::EntityNodeWidget2D(EntityNode3D* node,
        GraphEditor* editor,
        const glm::vec3& worldPos)
        : NodeWidget2D(node, editor, worldPos),
        entityNode(node) {
    }

    void EntityNodeWidget2D::on_right_click() {
        std::vector<ui::sContextMenuOption> options = {
            { "Set Entity", FuncInt([this](const std::string&, int) {
                show_entity_selector();
            })},
            { "Clear", FuncInt([this](const std::string&, int) {
                entityNode->setEntity(nullptr);
            })}
        };

        glm::vec2 screen_pos = get_translation();
        glm::vec3 world_pos = glm::vec3(screen_pos, 0.0f);

        auto* cm = new ui::ContextMenu(screen_pos, world_pos, options);
        SampleEngine::get_sample_instance()->push_context_menu(cm);
    }

    void EntityNodeWidget2D::show_entity_selector() {
        std::vector<ui::sContextMenuOption> entityOptions;

        auto all_nodes = Engine::get_instance()->get_main_scene()->get_nodes();

        for (Node* n : all_nodes) {
            auto* mesh = dynamic_cast<MeshInstance3D*>(n);
            if (!mesh) continue;

            std::string label = mesh->get_name();
            entityOptions.push_back({
                label,
                FuncInt([this, mesh](const std::string&, int) {
                    entityNode->setEntity(mesh);
                })
                });
        }

        glm::vec2 screen_pos = get_translation() + glm::vec2(150, 0);
        glm::vec3 world_pos = glm::vec3(screen_pos, 0.0f);

        auto* cm = new ui::ContextMenu(screen_pos, world_pos, entityOptions);
        SampleEngine::get_sample_instance()->push_context_menu(cm);
    }

}
