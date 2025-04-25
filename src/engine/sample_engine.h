#pragma once

#include "engine/engine.h"
#include "graph/graph_manager.h"
#include "graph/graph_editor.h"

class Node3D;

class SampleEngine : public Engine {

    Node3D* controller_mesh_left = nullptr;
    Node3D* controller_mesh_right = nullptr;

private:
    GraphSystem::GraphManager graphManager;
    GraphSystem::GraphEditor* editor = nullptr;
    std::vector<GraphSystem::NodeWidget2D*> nodeWidgets;
    
public:

	int initialize(Renderer* renderer, sEngineConfiguration configuration = {}) override;
    int post_initialize() override;
    void clean() override;
    void setupGraphUI();
    void setupNodeCreationUI(GraphSystem::GraphEditor* editor);
    void registerNodeWidget(GraphSystem::NodeWidget2D* w) { nodeWidgets.push_back(w); }

    static SampleEngine* get_sample_instance() { return static_cast<SampleEngine*>(instance); }

	void update(float delta_time) override;
	void render() override;
    void on_frame() override;

#ifdef __EMSCRIPTEN__
    void set_wasm_module_initialized(bool value) {
        wasm_module_initialized = value;
    }
#endif
};
