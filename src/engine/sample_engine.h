#pragma once

#include "engine/engine.h"
#include "graph/graph_manager.h"
#include "graph/graph_editor.h"
#include "framework/nodes/panel_2d.h"

namespace ui {
    class ContextMenu; 
}

class Node3D;
class MeshInstance3D;

class SampleEngine : public Engine {

    Node3D* controller_mesh_left = nullptr;
    Node3D* controller_mesh_right = nullptr;
    MeshInstance3D* ray_pointer = nullptr;
    MeshInstance3D* sphere_pointer = nullptr;

private:
    GraphSystem::GraphManager graphManager;
    GraphSystem::GraphEditor* editor = nullptr;
    std::vector<GraphSystem::NodeWidget2D*> nodeWidgets;
    ui::Panel2D* run_panel = nullptr;
    Node2D* graph_container = nullptr;

    std::vector<ui::ContextMenu*> context_menus;

    ui::ContextMenu* active_context_menu = nullptr;

   
    Node3D* scene_root = nullptr;

public:

	int initialize(Renderer* renderer, const sEngineConfiguration& configuration = {}) override;
    int post_initialize() override;
    void clean() override;
    void setupGraphUI();
    void setupNodeCreationUI(GraphSystem::GraphEditor* editor);
    void registerNodeWidget(GraphSystem::NodeWidget2D* w) { nodeWidgets.push_back(w); }

    static SampleEngine* get_sample_instance() { return static_cast<SampleEngine*>(instance); }
    Node2D* get_graph_container() const { return graph_container; }


    void push_context_menu(ui::ContextMenu* cm);
    void delete_context_menu(ui::ContextMenu* cm);



	void update(float delta_time) override;
	void render() override;
    void on_frame() override;

    void append_glb(const std::string& filename);

#ifdef __EMSCRIPTEN__
    void set_wasm_module_initialized(bool value) {
        wasm_module_initialized = value;
    }
#endif
};
