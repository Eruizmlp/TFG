#include "link_renderer_2d.h"
#include "port_button_2d.h"
#include "framework/colors.h"
#include "graph/graph_editor.h"
 


using namespace GraphSystem;
using namespace ui;

LinkRenderer2D::LinkRenderer2D(Graph* g, GraphEditor* ed, float thickness)
    : Node2D("LinkRenderer2D", { 0.0f, 0.0f }, { 0.0f, 0.0f }, ui::CREATE_3D)
    , graph(g)
    , editor(ed)
    , linkThickness(thickness)
{

}

LinkRenderer2D::~LinkRenderer2D() {
    clearLinks();
}

void LinkRenderer2D::update(float delta_time) {
    clearLinks();
    buildLinks();
}

void LinkRenderer2D::clearLinks() {
    for (auto* link : links) {
        remove_child(link);
    }
    links.clear();
}



void LinkRenderer2D::buildLinks() {
    for (auto* link : graph->getLinks()) {
        const std::string outId = "outBtn_" + link->getSourceNode()->getName()
            + "_" + link->getOutput()->getName();
        const std::string inId = "inBtn_" + link->getTargetNode()->getName()
            + "_" + link->getTargetInput()->getName();

        auto* outB = dynamic_cast<PortButton2D*>(Node2D::get_widget_from_name(outId));
        auto* inB = dynamic_cast<PortButton2D*>(Node2D::get_widget_from_name(inId));
        if (!outB || !inB) continue;

        glm::vec2 p0 = outB->get_translation() + outB->get_size() * 0.5f;
        glm::vec2 p1 = inB->get_translation() + inB->get_size() * 0.5f;

        auto* linkQuad = new LinkQuad(
            "link_" + link->getSourceNode()->getName() + "_" + link->getTargetNode()->getName(),
            p0,
            p1,
            linkThickness,
            colors::RUST 
        );

        linkQuad->set_viewport_model(get_global_viewport_model());

        add_child(linkQuad);
        links.push_back(linkQuad);
    }
}

