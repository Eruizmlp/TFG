//#include "set_variable_node.h"
//#include "variable_node.h" 
//#include <iostream>
//
//namespace GraphSystem {
//
//    SetVariableNode::SetVariableNode(const std::string& name, const std::string& variableName)
//        : GraphNode(name, NodeCategory::FLOW), targetVariable(variableName)
//    {
//        execInput = addInput("Execute", IOType::EXECUTION);
//        // El tipo de entrada es un hint, pero aceptará cualquier VariableValue.
//        valueInput = addInput("Value", IOType::FLOAT);
//        execOutput = addOutput("Exec", IOType::EXECUTION);
//    }
//
//    void SetVariableNode::setTargetVariable(const std::string& variableName) {
//        targetVariable = variableName;
//    }
//
//    const std::string& SetVariableNode::getTargetVariable() const {
//        return targetVariable;
//    }
//
//    void SetVariableNode::execute() {
//        if (!isExecutionPending()) return;
//        setExecutionPending(false);
//
//        if (valueInput && valueInput->hasData()) {
//            VariableValue valToStore = valueInput->getValue();
//
//            if (valToStore.index() != std::variant_npos) {
//                VariableNode::setStoredValue(targetVariable, valToStore);
//            }
//            else {
//                std::cerr << "[SetVariableNode] (" << getName() << ") Input 'Value' has no valid data to store.\n";
//            }
//        }
//        else {
//            std::cerr << "[SetVariableNode] (" << getName() << ") Input 'Value' is not connected or has no data.\n";
//        }
//
//        for (auto& link : execOutput->getLinks()) {
//            if (auto* next = link->getTargetNode()) {
//                next->setExecutionPending(true);
//            }
//        }
//    }
//}
