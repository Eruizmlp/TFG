#include "visual_link_2d.h"
#include "port_button_2d.h"
#include "framework/colors.h"
#include "graph/graph_editor.h"



using namespace GraphSystem;
using namespace ui;

VisualLink2D::VisualLink2D(Graph* g, GraphEditor* ed, float thickness)
    : Node2D("VisualLink2D", { 0.0f, 0.0f }, { 0.0f, 0.0f }, CREATE_3D)
    , graph(g)
    , editor(ed)
    , linkThickness(thickness)
{

}

VisualLink2D::~VisualLink2D() {
    clearLinks();
}

void VisualLink2D::update(float delta_time) {
    clearLinks();
    buildLinks();

}

void VisualLink2D::clearLinks() {
    for (auto* panel : panels) {
        remove_child(panel);
    }
    panels.clear();
}

void VisualLink2D::buildLinks() {

    for (auto* link : graph->getLinks()) {
        // Build unique IDs for source and target port buttons
        const std::string outId = "outBtn_" + link->getSourceNode()->getName()
            + "_" + link->getOutput()->getName();
        const std::string inId = "inBtn_" + link->getTargetNode()->getName()
            + "_" + link->getTargetInput()->getName();

        auto* outB = dynamic_cast<PortButton2D*>(Node2D::get_widget_from_name(outId));
        auto* inB = dynamic_cast<PortButton2D*>(Node2D::get_widget_from_name(inId));
        if (!outB || !inB) continue;

        // Compute center positions of each port button
        glm::vec2 p0 = outB->get_translation() + outB->get_size() * 0.5f;
        glm::vec2 p1 = inB->get_translation() + inB->get_size() * 0.5f;
        glm::vec2 d = p1 - p0;
        float     len = glm::length(d);
        float     ang = atan2(d.y, d.x);

        // Calculate midpoint, panel size, and position
        glm::vec2 mid = (p0 + p1) * 0.5f;
        glm::vec2 size = { len, linkThickness };
        glm::vec2 pos = mid - size * 0.5f;

        auto* visualLink_p = new XRPanel(
            "link_" + link->getSourceNode()->getName()
            + "_" + link->getTargetNode()->getName(),
            pos,
            size,
            0u,
            colors::RUST
        );

        //TODO:
        // Manually rotate around its center pivot
        visualLink_p->translate(size * 0.5f);
        visualLink_p->rotate(ang);
        visualLink_p->translate(-size * 0.5f);

        add_child(visualLink_p);
        panels.push_back(visualLink_p);
    }


}
