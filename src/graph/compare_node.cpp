#include "compare_node.h"
#include <iostream>
#include <fstream>

namespace GraphSystem {

    CompareNode::CompareNode(const std::string& name, CompareOp op)
        : GraphNode(name, NodeCategory::LOGIC), operation(op)
    {
        aInput = addInput("A", IOType::FLOAT);
        bInput = addInput("B", IOType::FLOAT);
        resultOutput = addOutput("Result", IOType::BOOL);


        resultOutput->setComputeFunction([this]() -> VariableValue {
            float a = 0.0f;
            float b = 0.0f;

            if (aInput->hasData()) {
                VariableValue v = aInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) a = *pval;
                else std::cerr << "[CompareNode] (" << getName() << ") Type mismatch for input A. Expected FLOAT.\n";
            }

            if (bInput->hasData()) {
                VariableValue v = bInput->getValue();
                if (const float* pval = std::get_if<float>(&v)) b = *pval;
                else std::cerr << "[CompareNode] (" << getName() << ") Type mismatch for input B. Expected FLOAT.\n";
            }

            bool result = false;
            switch (operation) {
            case CompareOp::EQUAL:         result = (a == b); break;
            case CompareOp::NOT_EQUAL:     result = (a != b); break;
            case CompareOp::LESS:          result = (a < b); break;
            case CompareOp::GREATER:       result = (a > b); break;
            case CompareOp::LESS_EQUAL:    result = (a <= b); break;
            case CompareOp::GREATER_EQUAL: result = (a >= b); break;
            default:                       result = false; break;
            }
            return VariableValue(result);
            });

        resultOutput->setData(VariableValue(false));
    }

    void CompareNode::setOperation(CompareOp op) {
        operation = op;
    }

    CompareOp CompareNode::getOperation() const {
        return operation;
    }

    void CompareNode::execute(std::queue<GraphNode*>& executionQueue)
    {
    }

    void CompareNode::serialize(std::ofstream& file) {
        GraphNode::serialize(file);
        file.write(reinterpret_cast<const char*>(&operation), sizeof(operation));
    }

    void CompareNode::parse(std::ifstream& file) {
        GraphNode::parse(file);
        file.read(reinterpret_cast<char*>(&operation), sizeof(operation));
    }

    void CompareNode::rebindPins() {

        aInput = getInput("A");
        bInput = getInput("B");
        resultOutput = getOutput("Result");


        if (resultOutput) {
            resultOutput->setComputeFunction([this]() -> VariableValue {

                if (!aInput || !bInput) return false;

                float a = aInput->getFloat();
                float b = bInput->getFloat();

                bool result = false;
                switch (operation) {
                case CompareOp::EQUAL:         result = (a == b); break;
                case CompareOp::NOT_EQUAL:     result = (a != b); break;
                case CompareOp::LESS:          result = (a < b); break;
                case CompareOp::GREATER:       result = (a > b); break;
                case CompareOp::LESS_EQUAL:    result = (a <= b); break;
                case CompareOp::GREATER_EQUAL: result = (a >= b); break;
                default:                       result = false; break;
                }
                return result;
                });
        }
    }

}
