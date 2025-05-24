#pragma once

#include "framework/nodes/node_2d.h"
#include "graphics/mesh_instance.h"
#include "framework/colors.h"

class LinkQuad : public Node2D {
    MeshInstance* mesh = nullptr;
    Surface* surface = nullptr;

public:
    LinkQuad(const std::string& name,
        const glm::vec2& start,
        const glm::vec2& end,
        float thickness,
        const Color& c);

    ~LinkQuad();

    void update_quad(const glm::vec2& start,
        const glm::vec2& end,
        float thickness,
        const Color& c);

    void render() override;
};
