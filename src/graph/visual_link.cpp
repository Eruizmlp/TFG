//// visual_link.cpp
//#include "visual_link.h"
//#include "engine/engine.h"
//#include "engine/scene.h"
//#include "graphics/renderer_storage.h"
//#include "framework/ui/io.h"
//#include "framework/colors.h"
//#include "node_widget_2d.h"
//#include "port_button_2d.h"
//
//
//using namespace GraphSystem;
//using namespace ui;
//
//VisualLink::VisualLink(Graph* g, GraphEditor* e)
//    : Node2D("VisualLink", { 0,0 }, { 0,0 }, ui::CREATE_3D)
//    , graph(g)
//    , editor(e)
//{
// 
//    // Cargamos la superficie de quad y creamos un material blanco
//    quadSurface = RendererStorage::instance->get_surface("quad");
//    mat = new Material();
//    mat->set_type(MATERIAL_UNLIT);
//    mat->set_color(colors::WHITE);
//}
//
//VisualLink::~VisualLink() {
//    clearInstances();
//    delete mat;
//}
//
//void VisualLink::update(float) {
//    clearInstances();
//    createInstances();
//}
//
//void VisualLink::clearInstances() {
//    auto* scene = Engine::get_instance()->get_main_scene();
//    for (auto* inst : insts) {
//        scene->remove_node(inst);
//        delete inst;
//    }
//    insts.clear();
//}
//
//void VisualLink::createInstances() {
//    auto* scene = Engine::get_instance()->get_main_scene();
//    const float thickness = 4.0f;
//
//    for (auto* link : graph->getLinks()) {
//        // Buscamos los botones de origen y destino
//        std::string outName = "outBtn_" + link->getSourceNode()->getName() + "_" + link->getOutput()->getName();
//        std::string inName = "inBtn_" + link->getTargetNode()->getName() + "_" + link->getTargetInput()->getName();
//        auto* outBtn = dynamic_cast<PortButton2D*>(Node2D::get_widget_from_name(outName));
//        auto* inBtn = dynamic_cast<PortButton2D*>(Node2D::get_widget_from_name(inName));
//        if (!outBtn || !inBtn) continue;
//
//        // Cálculo de puntos en 2D
//        glm::vec2 p0 = outBtn->get_translation() + outBtn->get_size() * 0.5f;
//        glm::vec2 p1 = inBtn->get_translation() + inBtn->get_size() * 0.5f;
//
//        // Matriz TRS para escalar/rotar el quad unitario
//        glm::mat4 xf = makeTransform(p0, p1, thickness);
//
//        // Instanciamos el mesh y lo añadimos a la escena
//        auto* inst = new MeshInstance3D();
//        inst->set_name("Link");
//        inst->add_surface(quadSurface);
//        inst->set_surface_material_override(0, mat);
//        inst->set_transform(Transform::mat4_to_transform(xf));
//        scene->add_node(inst);
//
//        insts.push_back(inst);
//    }
//}
//
//glm::mat4 VisualLink::makeTransform(const glm::vec2& p0,
//    const glm::vec2& p1,
//    float thickness)
//{
//    glm::vec2  mid = 0.5f * (p0 + p1);
//    glm::vec2  d = p1 - p0;
//    float      len = glm::length(d);
//    float      ang = atan2(d.y, d.x);
//
//    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(mid, 0.0f));
//    glm::mat4 R = glm::rotate(glm::mat4(1.0f), ang, glm::vec3(0, 0, 1));
//    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(len, thickness, 1.0f));
//    return T * R * S;
//}
