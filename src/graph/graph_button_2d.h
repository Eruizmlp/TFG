#pragma once

#include "framework/nodes/button_2d.h"
#include "../graph/graph.h"
#include <iostream>

namespace GraphSystem {

    class GraphButton2D : public ui::Button2D {
    private:
        Graph* graph; 

    public:
      
        GraphButton2D(const std::string& signal, const ui::sButtonDescription& desc, Graph* graph);
        bool on_pressed() override;
    };

} // namespace GraphSystem
