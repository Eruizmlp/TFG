#include "create_node_button.h"

namespace GraphSystem {

    CreateNodeButton::CreateNodeButton(
        const std::string& signal,
        const ui::sButtonDescription& desc,
        GraphEditor* editor,
        const std::string& nodeType)
        : ui::TextureButton2D(signal, desc), editor(editor), node_to_create(nodeType)
    {
    }

    bool CreateNodeButton::on_pressed() {
        if (editor && !node_to_create.empty()) {
            
            static int counter = 0; 
                std::string nodeName = node_to_create + "_" + std::to_string(counter++); 
                editor->createNode(node_to_create, nodeName); 
                return true;
        }
        return false;
    }
}
