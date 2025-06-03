#ifndef LINK_H
#define LINK_H

#include "io.h"

namespace GraphSystem {

    class GraphNode;  // Forward declaration

    class Link {
    private:
        Output* outputRef;
        GraphNode* targetNode;
        Input* targetInput;  // Added target input

    public:
        Link(Output* output, GraphNode* target, Input* input = nullptr);
        ~Link();

        // Getters
        Output* getOutput() const { return outputRef; }
        GraphNode* getTargetNode() const { return targetNode; }
        Input* getTargetInput() const { return targetInput; }
        GraphNode* getSourceNode() const;  // Added declaration

        void transferData();
    };

} // namespace GraphSystem

#endif // LINK_H
