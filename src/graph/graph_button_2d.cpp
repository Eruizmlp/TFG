#include "graph_button_2d.h"
#include "event_node.h"

namespace GraphSystem {

    // Constructor Implementation
    GraphButton2D::GraphButton2D(const std::string& signal, const ui::sButtonDescription& desc, Graph* graph)
        : Button2D(signal, desc), graph(graph) {} 

    bool GraphButton2D::on_pressed() {
        std::cout << "[GraphButton2D] Button Pressed! Searching for Event Nodes...\n";

        if (!graph) {
            std::cerr << "[Error] Graph is null! Execution aborted.\n";
            return false;
        }

        bool eventTriggered = false;

        // Find and trigger EventNodes in the graph
        for (const auto& node : graph->getNodes()) {
            auto eventNode = dynamic_cast<EventNode*>(node);
            if (eventNode) {
                std::cout << "[GraphButton2D] Triggering Event Node: " << eventNode->getEventName() << "\n";
                eventNode->setExecutionPending(true);
                eventTriggered = true;
            }
        }

        if (!eventTriggered) {
            std::cerr << "[Error] No Event Nodes Found! Execution aborted.\n";
            return false;
        }

        // Execute the graph after marking event nodes
        graph->execute();
        return true;
    }

} 
