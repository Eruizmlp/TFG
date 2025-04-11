#include "node_widget_3d.h"
#include "shaders/mesh_forward.wgsl.gen.h"


namespace GraphSystem {

    NodeWidget3D::NodeWidget3D(GraphNode* node)
        : logic_node(node)
    {
        set_name("Widget_" + node->getName());

        // Create MeshInstance3D as visual
        visual = new MeshInstance3D();
        visual->set_name("VisualBox");
        visual->scale(glm::vec3(0.2f));
        visual->set_position(glm::vec3(0.0f));  // local position inside NodeWidget3D

        // Get surface
        Surface* surf = RendererStorage::get_surface("box");
        visual->add_surface(surf);

        // Create material
        Material* mat = new Material();
        mat->set_transparency_type(ALPHA_OPAQUE);
        mat->set_cull_type(CULL_BACK);
        mat->set_type(MATERIAL_PBR);
        mat->set_color(glm::vec4(0.2f, 0.6f, 1.0f, 1.0f));  // Light blue

        // Load shader
        Shader* shader = RendererStorage::get_shader_from_source(
            shaders::mesh_forward::source,
            shaders::mesh_forward::path,
            shaders::mesh_forward::libraries,
            mat
        );
        mat->set_shader(shader);

        // Apply material
        visual->set_surface_material_override(visual->get_surface(0), mat);

        // Attach visual box to this node
        add_child(visual);
    }

    void NodeWidget3D::set_position_world(const glm::vec3& pos) {
        set_position(pos);
    }

    void NodeWidget3D::highlight(bool enable) {
        if (!visual) return;

        Material* mat = visual->get_surface_material_override(0);
        if (!mat) return;

        if (enable) {
            mat->set_color(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));  // Yellow highlight
        }
        else {
            mat->set_color(glm::vec4(0.2f, 0.6f, 1.0f, 1.0f));  // Default blue
        }
    }

}
