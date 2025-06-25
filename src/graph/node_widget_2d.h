#pragma once

#include <glm/glm.hpp>
#include "framework/nodes/node_2d.h"
#include "framework/nodes/container_2d.h"
#include "framework/nodes/text.h"
#include "framework/nodes/button_2d.h"
#include "framework/nodes/slider_2d.h"
#include "framework/colors.h"

#include "graph_node.h"
#include "graph_editor.h"
#include "port_button_2d.h"
#include "rotate_node.h"
#include "scale_node.h"
#include "translate_node.h"

namespace GraphSystem {

    class NodeWidget2D : public Node2D {
    protected:
        GraphNode* logic_node;
        GraphEditor* graphEditor;
        ui::XRPanel* background = nullptr;
        ui::VContainer2D* rootContainer;
        bool              dragging = false;
        glm::vec2         dragOffset;
        std::string       graphNodeType;

        ui::Text2D* titleLabel = nullptr;

        bool      m_is_grabbing = false;
        glm::mat4 m_grab_offset_transform;

        static glm::vec2 computeSize(GraphNode* node);

    public:
        NodeWidget2D(const std::string& nodeType,
            GraphNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        GraphNode* getLogicNode() const { return logic_node; }
        const std::string& getGraphNodeType() const { return graphNodeType; }
        static std::string formatFloat(float value, int precision = 2);
        static glm::vec4 getColorForCategory(GraphSystem::NodeCategory cat);

        // input / render hooks
        bool       on_input(sInputData data) override;
        void       update(float delta_time) override;
        
        sInputData get_input_data(bool ignore_focus = false) override;

        // inspector hooks
        virtual void toggleInspector(sInputData data) {}
        virtual void updateInspector() {}

        virtual void on_right_click() {}

        void updateTitleFromLogicNode();
        void rebuildWidgetUI();
        virtual void serialize(std::ofstream& binary_scene_file);
        virtual void parse(std::ifstream& binary_scene_file);



    };

    class RotateNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        ui::FloatSlider2D* angleSlider = nullptr;
        ui::Text2D* angleValueLabel = nullptr;
        ui::Button2D* axisXBtn = nullptr;
        ui::Button2D* axisYBtn = nullptr;
        ui::Button2D* axisZBtn = nullptr;

        bool  inspectorVisible = false;

        void initInspector();

    public:
        RotateNodeWidget2D(const std::string& nodeType,
            RotateNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;

        void serialize(std::ofstream& binary_scene_file) override;
        void parse(std::ifstream& binary_scene_file) override;
    };

    
    class TranslateNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        ui::FloatSlider2D* xSlider = nullptr;
        ui::FloatSlider2D* ySlider = nullptr;
        ui::FloatSlider2D* zSlider = nullptr;

        ui::Text2D* xValueLabel = nullptr;
        ui::Text2D* yValueLabel = nullptr;
        ui::Text2D* zValueLabel = nullptr;

        bool          inspectorVisible = false;

        void initInspector();
    public:
        TranslateNodeWidget2D(const std::string& nodeType,
            TranslateNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;

        void serialize(std::ofstream& binary_scene_file);
        void parse(std::ifstream& binary_scene_file);
    };

   
    class ScaleNodeWidget2D : public NodeWidget2D {
    private:
        ui::XRPanel* inspectPanel = nullptr;
        ui::FloatSlider2D* factorSlider = nullptr;
        ui::Text2D* scaleValueLabel = nullptr;
        bool             inspectorVisible = false;

        void initInspector();
    public:
        ScaleNodeWidget2D(const std::string& nodeType,
            ScaleNode* node,
            GraphEditor* editor,
            const glm::vec3& worldPos);

        void toggleInspector(sInputData data) override;
        void updateInspector() override;
        void update(float delta_time) override;

        void serialize(std::ofstream& binary_scene_file);
        void parse(std::ifstream& binary_scene_file);

    };


} 
