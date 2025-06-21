#include "clamp_node.h"
#include <algorithm>
#include <iostream>
#include <fstream> 

namespace GraphSystem {

    ClampNode::ClampNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        valueInput = addInput("Value", IOType::FLOAT);
        minInput = addInput("Min", IOType::FLOAT);
        maxInput = addInput("Max", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);


        valueInput->setData(defaultValue);
        minInput->setData(defaultMin);
        maxInput->setData(defaultMax);

        resultOutput->setComputeFunction([this]() -> VariableValue {
            float val = valueInput->getFloat();
            float min_val = minInput->getFloat();
            float max_val = maxInput->getFloat();
            return std::clamp(val, min_val, max_val);
            });

        resultOutput->setData(std::clamp(defaultValue, defaultMin, defaultMax));
    }

    void ClampNode::execute(std::queue<GraphNode*>& executionQueue) {

    }

    void ClampNode::serialize(std::ofstream& file) {
        
        GraphNode::serialize(file);
        file.write(reinterpret_cast<const char*>(&defaultValue), sizeof(defaultValue));
        file.write(reinterpret_cast<const char*>(&defaultMin), sizeof(defaultMin));
        file.write(reinterpret_cast<const char*>(&defaultMax), sizeof(defaultMax));
    }

    void ClampNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
        file.read(reinterpret_cast<char*>(&defaultValue), sizeof(defaultValue));
        file.read(reinterpret_cast<char*>(&defaultMin), sizeof(defaultMin));
        file.read(reinterpret_cast<char*>(&defaultMax), sizeof(defaultMax));
    }

    void ClampNode::rebindPins() {
        valueInput = getInput("Value");
        minInput = getInput("Min");
        maxInput = getInput("Max");
        resultOutput = getOutput("Result");

        if (valueInput) valueInput->setData(defaultValue);
        if (minInput) minInput->setData(defaultMin);
        if (maxInput) maxInput->setData(defaultMax);


        if (resultOutput) {
            resultOutput->setComputeFunction([this]() -> VariableValue {
                if (!valueInput || !minInput || !maxInput) return 0.0f;

                float val = valueInput->getFloat();
                float min_val = minInput->getFloat();
                float max_val = maxInput->getFloat();
                return std::clamp(val, min_val, max_val);
                });
        }
    }
}
