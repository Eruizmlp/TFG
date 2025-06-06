//#include "start_node.h"
//#include <iostream>
//
//namespace GraphSystem {
//
//    StartNode::StartNode(const std::string& name)
//        : GraphNode(name) {
//        setEntryPoint(true);
//        setExecutionPending(true);
//        addOutput("Start", GraphSystem::IOType::EXECUTION); 
//    }
//
//    void StartNode::execute() {
//        std::cout << "[StartNode] Execution Started!\n";
//
//        // Propagar ejecución a través del output
//        for (auto& output : getOutputs()) {
//            if (output->getType() == IOType::EXECUTION) {
//                for (auto& link : output->getLinks()) {
//                    if (auto targetNode = link->getTargetNode()) {
//                        std::cout << "[StartNode] Triggering connected node\n";
//                        targetNode->setExecutionPending(true);
//                    }
//                }
//            }
//        }
//
//        setExecutionPending(false);
//    }
//}
