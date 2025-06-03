#include "mapper_node.h"
#include "framework/input.h"

namespace GraphSystem {

    MapperNode::MapperNode(const std::string& name)
        : GraphNode(name, NodeCategory::INTERACTION), buttonCode(GLFW_KEY_A) // por defecto
    {
        isPressedOutput = addOutput("IsPressed", IOType::BOOL);
        wasPressedOutput = addOutput("WasPressed", IOType::BOOL);
    }

    void MapperNode::update(float deltaTime) {
        isPressedOutput->setData(::Input::is_key_pressed(buttonCode));
        wasPressedOutput->setData(::Input::was_key_pressed(buttonCode));
    }

    void MapperNode::setMappedButton(int button) {
        buttonCode = button;
    }

    int MapperNode::getMappedButton() const {
        return buttonCode;
    }

    void MapperNode::execute()
    {
    }

}
