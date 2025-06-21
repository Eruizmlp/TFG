#pragma once
#include "graph_node.h"
#include <queue>
#include <fstream>

namespace GraphSystem {

    enum class MappedButton {
        A,
        B,
        X,
        Y
    };

    enum VRButtonIndex {
        VR_BTN_A_INDEX = 0,
        VR_BTN_B_INDEX = 1,
        VR_BTN_X_INDEX = 2,
        VR_BTN_Y_INDEX = 3
    };

    class MapperNode : public GraphNode {
    public:
        explicit MapperNode(const std::string& name, MappedButton defaultButton = MappedButton::A);

        void update(float deltaTime) override;
        void execute(std::queue<GraphNode*>& executionQueue) override;

        void setMappedButton(MappedButton button);

        void serialize(std::ofstream& file) override;
        void parse(std::ifstream& file) override;
        void rebindPins() override;

    private:
        Output* isPressedOutput = nullptr;
        Output* wasPressedOutput = nullptr;
        MappedButton mappedButton;
    };
}
