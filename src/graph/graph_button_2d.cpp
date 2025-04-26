#include "graph_button_2d.h"
#include "event_node.h"

namespace GraphSystem {

    GraphButton2D::GraphButton2D(const std::string& signal,
        const ui::sButtonDescription& desc,
        Graph* graph,
        GraphEditor* editor,
        const std::string& nodeType)
        : Button2D(signal, desc), graph(graph), editor(editor), node_to_create(nodeType) {
    }

    bool GraphButton2D::on_pressed() {
        std::cout << "[GraphButton2D] Button Pressed!\n";

        if (editor && !node_to_create.empty()) {
            static int counter = 0;
            std::string nodeName = node_to_create + "_" + std::to_string(counter++);
            auto* node = editor->createNode(node_to_create, nodeName);
            if (node) {
                std::cout << "[GraphButton2D] Created node: " << nodeName << "\n";
            }
        }

        if (!graph) {
            std::cerr << "[Error] Graph is null!\n";
            return false;
        }

        bool eventTriggered = false;
        for (const auto& node : graph->getNodes()) {
            auto eventNode = dynamic_cast<EventNode*>(node);
            if (eventNode) {
                std::cout << "[GraphButton2D] Triggering Event Node: " << eventNode->getEventName() << "\n";
                eventNode->setExecutionPending(true);
                eventTriggered = true;
            }
        }

        if (!eventTriggered) {
            std::cerr << "[Error] No Event Nodes Found!\n";
            return false;
        }

        graph->execute();
        return true;
    }

}
