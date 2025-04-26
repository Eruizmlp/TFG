#pragma once

#include "framework/nodes/button_2d.h"
#include "../graph/graph_node.h"
#include "../graph/graph_editor.h"
#include <string>

namespace GraphSystem {

    
    class PortButton2D : public ui::Button2D {
    private:
        GraphNode* logic_node;   
        GraphEditor* graphEditor;  
        std::string   portName;     
        bool          isOutput;     

    public:

        PortButton2D(const std::string& id,
            const ui::sButtonDescription& desc,
            GraphNode* node,
            GraphEditor* editor,
            const std::string& port,
            bool output);

        bool on_pressed() override;
    };

}
