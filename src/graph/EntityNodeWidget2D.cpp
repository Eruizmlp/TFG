
#include "entity_node_widget_2d.h"
#include "graphics/renderer.h"
#include "context_menu.h"
#include "engine/sample_engine.h"

EntityNodeWidget2D::EntityNodeWidget2D(GraphSystem::GraphNode* node)
    : NodeWidget2D(node) {
}

void EntityNodeWidget2D::on_right_click() {
    auto* entityNode = dynamic_cast<GraphSystem::EntityNode3D*>(graph_node);
    if (!entityNode) return;

    std::vector<sContextMenuOption> options;

    for (auto* node : SampleEngine::get_sample_instance()->get_main_scene()->get_children()) {
        auto* mesh = dynamic_cast<MeshInstance3D*>(node);
        if (!mesh) continue;

        options.push_back(sContextMenuOption{
            mesh->get_name(),
            [entityNode, mesh](const std::string&, int) {
                entityNode->setEntity(mesh);
            }
            });
    }

    glm::vec2 mouse = Input::get_mouse_position();
    glm::vec3 world = Renderer::instance->screen_to_world(mouse);

    auto* menu = new ui::ContextMenu(mouse, world, options);
    SampleEngine::get_sample_instance()->push_context_menu(menu);
}
