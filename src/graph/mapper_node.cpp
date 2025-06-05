#include "mapper_node.h"
#include "framework/input.h" 

namespace GraphSystem {

    MapperNode::MapperNode(const std::string& name)
        : GraphNode(name, NodeCategory::INTERACTION), buttonCode(GLFW_KEY_A)
    {
        isPressedOutput = addOutput("IsPressed", IOType::BOOL); 
        wasPressedOutput = addOutput("WasPressed", IOType::BOOL); 

        
        isPressedOutput->setData(VariableValue(false)); 
        wasPressedOutput->setData(VariableValue(false));
    }

    void MapperNode::update(float deltaTime) { 
        isPressedOutput->setData(VariableValue(::Input::is_key_pressed(buttonCode))); 
        wasPressedOutput->setData(VariableValue(::Input::was_key_pressed(buttonCode)));  
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
