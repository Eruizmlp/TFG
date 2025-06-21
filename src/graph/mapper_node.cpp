#include "mapper_node.h"
#include "framework/input.h"

namespace GraphSystem {


    MapperNode::MapperNode(const std::string& name, MappedButton defaultButton)
        : GraphNode(name, NodeCategory::INTERACTION), mappedButton(defaultButton)
    {
        isPressedOutput = addOutput("IsPressed", IOType::BOOL);
        wasPressedOutput = addOutput("WasPressed", IOType::BOOL);
        isPressedOutput->setData(false);
        wasPressedOutput->setData(false);
    }

    void MapperNode::update(float deltaTime) {
        bool isPressed = false;
        bool wasPressed = false;

        switch (mappedButton) {
        case MappedButton::A:
        
            isPressed = ::Input::is_button_pressed(VR_BTN_A_INDEX) || ::Input::is_key_pressed(GLFW_KEY_A);
            wasPressed = ::Input::was_button_pressed(VR_BTN_A_INDEX) || ::Input::was_key_pressed(GLFW_KEY_A);
            break;

        case MappedButton::B:
            isPressed = ::Input::is_button_pressed(VR_BTN_B_INDEX) || ::Input::is_key_pressed(GLFW_KEY_B);
            wasPressed = ::Input::was_button_pressed(VR_BTN_B_INDEX) || ::Input::was_key_pressed(GLFW_KEY_B);
            break;

        case MappedButton::X:
            isPressed = ::Input::is_button_pressed(VR_BTN_X_INDEX) || ::Input::is_key_pressed(GLFW_KEY_X);
            wasPressed = ::Input::was_button_pressed(VR_BTN_X_INDEX) || ::Input::was_key_pressed(GLFW_KEY_X);
            break;

        case MappedButton::Y:
            isPressed = ::Input::is_button_pressed(VR_BTN_Y_INDEX) || ::Input::is_key_pressed(GLFW_KEY_Y);
            wasPressed = ::Input::was_button_pressed(VR_BTN_Y_INDEX) || ::Input::was_key_pressed(GLFW_KEY_Y);
            break;
        }

        isPressedOutput->setData(isPressed);
        wasPressedOutput->setData(wasPressed);
    }

    void MapperNode::setMappedButton(MappedButton button) {
        this->mappedButton = button;
    }

    void MapperNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
        file.write(reinterpret_cast<const char*>(&mappedButton), sizeof(mappedButton));
    }

    void MapperNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
        file.read(reinterpret_cast<char*>(&mappedButton), sizeof(mappedButton));
    }

    void MapperNode::rebindPins() {
        isPressedOutput = getOutput("IsPressed");
        wasPressedOutput = getOutput("WasPressed");
    }

    void MapperNode::execute(std::queue<GraphNode*>& executionQueue) {}
}
