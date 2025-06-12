#pragma once

#include "framework/nodes/button_2d.h"
#include "graph_editor.h"

namespace GraphSystem {
    
    class CreateNodeButton : public ui::TextureButton2D {
    private:
        GraphEditor* editor;
        std::string node_to_create;

    public:
        CreateNodeButton(
            const std::string& signal,
            const ui::sButtonDescription& desc,
            GraphEditor* editor,
            const std::string& nodeType
        );

        bool on_pressed() override;
    };
}
