#include "loop_node.h"
#include <iostream>
#include <queue>

namespace GraphSystem {

    // El constructor es correcto y no necesita cambios.
    LoopNode::LoopNode(const std::string& name)
        : GraphNode(name, NodeCategory::FLOW)
    {
        addInput("Execute", IOType::EXECUTION);
        countInput = addInput("Count", IOType::FLOAT);

        loopExecOutput = addOutput("LoopExec", IOType::EXECUTION);
        completedOutput = addOutput("Completed", IOType::EXECUTION);
        indexOutput = addOutput("Index", IOType::FLOAT);

        indexOutput->setData(VariableValue(0.0f));
    }

    void LoopNode::execute(std::queue<GraphNode*>& executionQueue) {

        
        int repeatCount = 0;
        if (countInput && countInput->hasData()) {
            VariableValue v = countInput->getValue();
            if (const float* pval = std::get_if<float>(&v)) {
                repeatCount = static_cast<int>(*pval);
            }
            else {
                std::cerr << "[LoopNode] (" << getName() << ") Type mismatch for Count input. Expected FLOAT.\n";
            }
        }

        if (repeatCount > 0) {
            for (int i = 0; i < repeatCount; ++i) {

                indexOutput->setData(VariableValue(static_cast<float>(i)));

                for (auto* link : loopExecOutput->getLinks()) {
                    if (auto* nextNode = link->getTargetNode()) {
                        executionQueue.push(nextNode);
                    }
                }
            }
        }

        for (auto* link : completedOutput->getLinks()) {
            if (auto* nextNode = link->getTargetNode()) {
                executionQueue.push(nextNode);
            }
        }
    }

    // El método update ya no es necesario para la lógica del bucle.
    void LoopNode::update(float deltaTime) {
        // Se puede dejar vacío.
    }
}
