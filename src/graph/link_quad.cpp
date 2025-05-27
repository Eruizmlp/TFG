#include "link_quad.h"
#include "graphics/material.h"
#include "graphics/renderer.h"
#include "shaders/mesh_forward.wgsl.gen.h"
#include "graphics/renderer_storage.h"
#include "spdlog/spdlog.h"

sSurfaceData generate_link_quad(const glm::vec2& start, const glm::vec2& end, float thickness, const glm::vec3& color) {
    glm::vec2 dir = glm::normalize(end - start);
    glm::vec2 normal = glm::vec2(-dir.y, dir.x); 
    glm::vec2 offset = 0.5f * thickness * normal;


    glm::vec2 p0 = start + offset; 
    glm::vec2 p1 = end + offset; 
    glm::vec2 p2 = end - offset; 
    glm::vec2 p3 = start - offset; 

    sSurfaceData surface;

    surface.vertices = {
        glm::vec3(p0, 0.0f),  
        glm::vec3(p1, 0.0f),  
        glm::vec3(p2, 0.0f),  

        glm::vec3(p2, 0.0f),  
        glm::vec3(p3, 0.0f),  
        glm::vec3(p0, 0.0f)   
    };

    surface.normals = std::vector<glm::vec3>(6, glm::vec3(0.0f, 0.0f, 1.0f));
    surface.colors = std::vector<glm::vec3>(6, color);

    return surface;
}


LinkQuad::LinkQuad(const std::string& name,
    const glm::vec2& start,
    const glm::vec2& end,
    float thickness,
    const Color& c)
    : Node2D(name, { 0, 0 }, { 1.0f, 1.0f }, 0u)
{
 
    Material* mat = new Material();
    mat->set_color(c);
    mat->set_type(MATERIAL_UNLIT);
    mat->set_is_2D(true);
    mat->set_cull_type(CULL_BACK);
    mat->set_transparency_type(ALPHA_BLEND);
    mat->set_depth_read_write(false);
    mat->set_shader(RendererStorage::get_shader_from_source(
        shaders::mesh_forward::source,
        shaders::mesh_forward::path,
        shaders::mesh_forward::libraries,
        mat
    ));

    surface = new Surface();
    sSurfaceData data = generate_link_quad(start, end, thickness, glm::vec3(c.r, c.g, c.b));
    surface->create_surface_data(data, true);

    mesh = new MeshInstance();
    mesh->add_surface(surface);
    mesh->set_surface_material_override(surface, mat);
}

LinkQuad::~LinkQuad() {
    delete mesh;
    delete surface;
}

void LinkQuad::update_quad(const glm::vec2& start,
    const glm::vec2& end,
    float thickness,
    const Color& c)
{
    if (!surface) return;
    sSurfaceData data = generate_link_quad(start, end, thickness, glm::vec3(c.r, c.g, c.b));
    surface->update_surface_data(data, true);
}

void LinkQuad::render() {
    if (!mesh) return;

    Renderer::instance->add_renderable(mesh, glm::mat4(1.0f));
    Node2D::render();
}
