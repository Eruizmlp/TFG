//// visual_link.h
//#pragma once
//
//#include "framework/nodes/node_2d.h"
//#include "graph.h"
//#include "graph_editor.h"
//#include <vector>
//
//namespace GraphSystem {
//
//    class VisualLink : public Node2D {
//    public:
//        VisualLink(Graph* graph, GraphEditor* editor);
//        ~VisualLink() override;
//
//        // Actualiza/crea los quads cada frame
//        void update(float delta_time) override;
//
//    private:
//        Graph* graph;
//        GraphEditor* editor;
//        std::vector<MeshInstance3D*> insts;
//        Surface* quadSurface = nullptr;
//        Material* mat = nullptr;
//
//        // Helper para construir la matriz TRS de un quad 1×1 en p0→p1
//        glm::mat4 makeTransform(const glm::vec2& p0,
//            const glm::vec2& p1,
//            float thickness);
//
//        void clearInstances();
//        void createInstances();
//    };
//
//}
