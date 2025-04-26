#include "port_button_2d.h"
#include <iostream>

using namespace GraphSystem;

PortButton2D::PortButton2D(const std::string& id,
    const ui::sButtonDescription& desc,
    GraphNode* node,
    GraphEditor* editor,
    const std::string& port,
    bool output)
    : ui::Button2D(id, desc)
    , logic_node(node)
    , graphEditor(editor)
    , portName(port)
    , isOutput(output)
{
}

bool PortButton2D::on_pressed() {
    std::cout << "[GraphPortButton2D] Port pressed: "
        << portName << " on node " << logic_node->getName() << "\n";

    if (!graphEditor) {
        std::cerr << "[GraphPortButton2D] GraphEditor is null!\n";
        return false;
    }

    if (isOutput)
        graphEditor->beginConnection(logic_node, portName);
    else
        graphEditor->completeConnection(logic_node, portName);

    return true;
}

