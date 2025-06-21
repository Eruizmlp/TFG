#include "lerp_node.h"
#include <iostream>
#include <fstream>          
#include <algorithm>        


namespace GraphSystem {

   
    LerpNode::LerpNode(const std::string& name)
        : GraphNode(name, NodeCategory::DATA)
    {
        
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        tInput = addInput("T", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::FLOAT);

       
        aInput->setData(defaultA);
        bInput->setData(defaultB);
        tInput->setData(defaultT);

       
        resultOutput->setComputeFunction([this]() -> VariableValue {
            float a = aInput->getFloat();
            float b = bInput->getFloat();
            float t = tInput->getFloat();

            return glm::mix(a, b, std::clamp(t, 0.0f, 1.0f));
            });

        resultOutput->setData(glm::mix(defaultA, defaultB, std::clamp(defaultT, 0.0f, 1.0f)));
    }

   
    void LerpNode::execute(std::queue<GraphNode*>& executionQueue) {}



    void LerpNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
       
        file.write(reinterpret_cast<const char*>(&defaultA), sizeof(defaultA));
        file.write(reinterpret_cast<const char*>(&defaultB), sizeof(defaultB));
        file.write(reinterpret_cast<const char*>(&defaultT), sizeof(defaultT));
    }

    void LerpNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
     
        file.read(reinterpret_cast<char*>(&defaultA), sizeof(defaultA));
        file.read(reinterpret_cast<char*>(&defaultB), sizeof(defaultB));
        file.read(reinterpret_cast<char*>(&defaultT), sizeof(defaultT));
    }

    void LerpNode::rebindPins() {
  
        aInput = getInput("A");
        bInput = getInput("B");
        tInput = getInput("T");
        resultOutput = getOutput("Result");


        if (aInput) aInput->setData(defaultA);
        if (bInput) bInput->setData(defaultB);
        if (tInput) tInput->setData(defaultT);


        if (resultOutput) {
            resultOutput->setComputeFunction([this]() -> VariableValue {
                if (!aInput || !bInput || !tInput) return 0.0f; 

                float a = aInput->getFloat();
                float b = bInput->getFloat();
                float t = tInput->getFloat();
                return glm::mix(a, b, std::clamp(t, 0.0f, 1.0f));
                });
        }
    }
}
