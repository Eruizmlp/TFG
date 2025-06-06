//#pragma once
//#include "graph_node.h"
//#include "variable_node.h"
//
//namespace GraphSystem {
//
//    class SetVariableNode : public GraphNode {
//    public:
//        SetVariableNode(const std::string& name, const std::string& variableName = ""); 
//
//        void setTargetVariable(const std::string& variableName);  
//        const std::string& getTargetVariable() const;              
//
//        Output* execute() override;
//
//    private:
//        Input* execInput = nullptr;
//        Input* valueInput = nullptr;
//        Output* execOutput = nullptr;
//
//        std::string targetVariable;
//    };
//
//}
