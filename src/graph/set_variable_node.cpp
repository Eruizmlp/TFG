#include "set_variable_node.h"
#include "variable_node.h" 

namespace GraphSystem {

    SetVariableNode::SetVariableNode(const std::string& name, const std::string& variableName)
        : GraphNode(name, NodeCategory::FLOW), targetVariable(variableName)
    {
        execInput = addInput("Execute", IOType::EXECUTION); 
        valueInput = addInput("Value", IOType::FLOAT); 
        execOutput = addOutput("Exec", IOType::EXECUTION);  
    }

    void SetVariableNode::setTargetVariable(const std::string& variableName) {
        targetVariable = variableName; 
    }

    const std::string& SetVariableNode::getTargetVariable() const {
        return targetVariable; 
    }

    void SetVariableNode::execute() {
        if (!isExecutionPending()) return; 
        setExecutionPending(false);  

        if (valueInput && valueInput->getConnectedOutput() && valueInput->getConnectedOutput()->hasData()) { 

            VariableValue valToStore;
            
            Output* connected_out = valueInput->getConnectedOutput();
            if (connected_out) {
                auto val_func = connected_out->getComputeFunction();
                if (val_func) {
                    valToStore = val_func();
                }
                else {
                    valToStore = VariableValue(valueInput->getFloat());
                }
            }
            else if (valueInput->hasData()) { 
                valToStore = VariableValue(valueInput->getFloat()); 
            }


            if (valToStore.index() != std::variant_npos) { 
                VariableNode::setStoredValue(targetVariable, valToStore);  
            }
        }

        for (auto& link : execOutput->getLinks()) { 
            if (auto* next = link->getTargetNode()) {
                next->setExecutionPending(true); 
            }
        }
    }
}
