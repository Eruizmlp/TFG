#include "graph_editor.h"
#include "node_widget_2d.h"
#include "engine/engine.h"
#include "engine/scene.h"
#include "print_node.h"
//#include "sequence_node.h"
#include "clamp_node.h"
#include "compare_node.h"
#include "get_variable_node.h"
#include "lerp_node.h"
#include "loop_node.h"
#include "mapper_node.h"
#include "random_node.h"
#include "timer_node.h"
#include "toggle_node.h"
#include "mapper_node_widget_2d.h"
#include "compare_node_widget_2d.h"
#include "run_node.h"
#include "math_node.h"
#include "branch_node.h"
#include "variable_node.h"
#include "tick_node.h"
#include "tick_node_widget_2d.h"
#include <iostream>
#include "link_renderer_2d.h"
#include "math_node_widget_2d.h"
#include "entity_node_3d.h"
#include "entity_node_widget_2d.h"
#include "trigonometric_node.h"
#include "trigonometric_node_widget_2d.h"
//#include "set_variable_node.h"
#include "keyboard.h"
#include <fstream>
#include "spdlog/spdlog.h"

using namespace GraphSystem;


void serializeVariableValue(std::ofstream& file, const VariableValue& val) {
    uint8_t type_index = static_cast<uint8_t>(val.index());
    file.write(reinterpret_cast<const char*>(&type_index), sizeof(type_index));

    std::visit([&file](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            uint64_t size = arg.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));
            file.write(arg.c_str(), size);
        }
        else if constexpr (std::is_same_v<T, bool> ||
            std::is_same_v<T, int> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, glm::vec2> ||
            std::is_same_v<T, glm::vec3> ||
            std::is_same_v<T, glm::vec4> ||
            std::is_same_v<T, glm::mat4>) {
            file.write(reinterpret_cast<const char*>(&arg), sizeof(T));
        }
        // No se hace nada para MeshInstance3D*, ya que se maneja por referencia.
        // No se hace nada para EXECUTION, ya que no tiene valor.
        }, val);
}

VariableValue parseVariableValue(std::ifstream& file) {
    uint8_t type_index;
    file.read(reinterpret_cast<char*>(&type_index), sizeof(type_index));
    IOType type = static_cast<IOType>(type_index);

    switch (type) {
    case IOType::BOOL: { bool v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::INT: { int v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::FLOAT: { float v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::STRING: {
        uint64_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        std::string v(size, '\0');
        if (size > 0) file.read(&v[0], size);
        return v;
    }
    case IOType::VEC2: { glm::vec2 v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::VEC3: { glm::vec3 v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::VEC4: { glm::vec4 v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::MAT4: { glm::mat4 v; file.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; }
    case IOType::MESH:
    case IOType::EXECUTION:
    default:
        return {};
    }
}

GraphEditor::GraphEditor(Graph* graph, Node2D* panel)
    : graph(graph), graph_container(panel)
{
    visualLink = new LinkRenderer2D(graph, this);
    if (graph_container) {
        graph_container->add_child(visualLink);
    }
    else if (auto* eng = Engine::get_instance(); eng && eng->get_main_scene()) {
        eng->get_main_scene()->add_node(visualLink);
    }
}

GraphNode* GraphEditor::createNode(const std::string& type,
    const std::string& name,
    const glm::vec3& worldPosition)
{
    GraphNode* node = nullptr;
    std::string nodeName = name.empty()
        ? ("node_" + std::to_string(rand()))
        : name;

    if (type == "PrintNode")         node = new PrintNode(nodeName);
    else if (type == "RotateNode")   node = new RotateNode(nodeName);
   // else if (type == "SequenceNode") node = new SequenceNode(nodeName, 2);
    else if (type == "RunNode")    node = new RunNode(nodeName);
    else if (type == "MathNode")    node = new MathNode(nodeName);
    else if (type == "BranchNode")    node = new BranchNode(nodeName);
    else if (type == "TickNode")     node = new TickNode(nodeName);
    else if (type == "ScaleNode")    node = new ScaleNode(nodeName);
    else if (type == "EntityNode3D") node = new EntityNode3D(nodeName);
    else if (type == "TrigonometricNode") node = new TrigonometricNode(nodeName);
    else if (type == "TranslateNode") node = new TranslateNode(nodeName);
   // else if (type == "SetVariableNode") node = new SetVariableNode(nodeName, "");
    else if (type == "VariableNode")    node = new VariableNode(nodeName, "");
    else if (type == "ClampNode")       node = new ClampNode(nodeName);
    else if (type == "CompareNode")     node = new CompareNode(nodeName);
    else if (type == "GetVariableNode") node = new GetVariableNode(nodeName, ""); // Assumes default empty var name
    else if (type == "LerpNode")        node = new LerpNode(nodeName);
    else if (type == "LoopNode")        node = new LoopNode(nodeName);
    else if (type == "MapperNode")      node = new MapperNode(nodeName);
    else if (type == "RandomNode")      node = new RandomNode(nodeName);
    else if (type == "TimerNode")       node = new TimerNode(nodeName);
    else if (type == "ToggleNode")      node = new ToggleNode(nodeName);

    //else if (type == "VariableNode") {
    //    auto* varNode = new VariableNode(nodeName);

    //    ui::Keyboard::get_instance().request(
    //        [varNode](const std::string& userInput) {
    //            varNode->setVariableName(userInput);
    //        },
    //        "defaultVar",
    //        32
    //    );

    //    node = varNode;
    //}

    graph->addNode(node);

    if (!node) {
        std::cerr << "[GraphEditor] Unknown node type: " << type << "\n";
        return nullptr;
    }

    glm::vec3 localPosition = worldPosition;

    glm::vec2 containerPos = graph_container->get_translation();
    localPosition.x -= containerPos.x;
    localPosition.y -= containerPos.y;
        


    // choose the right widget subclass
    NodeWidget2D* widget = nullptr;
    if (auto* rn = dynamic_cast<RotateNode*>(node))
        widget = new RotateNodeWidget2D(type, rn, this, worldPosition);
    else if (auto tr = dynamic_cast<TranslateNode*>(node)) {
        widget = new TranslateNodeWidget2D(type, tr, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<ScaleNode*>(node)) {
        widget = new ScaleNodeWidget2D(type, sc, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<MathNode*>(node)) {
        widget = new MathNodeWidget2D(type, sc, this, worldPosition);
    }
    else if (auto sc = dynamic_cast<TrigonometricNode*>(node)) {
        widget = new TrigonometricNodeWidget2D(type, sc, this, worldPosition);
    }
    else if (auto* tk = dynamic_cast<TickNode*>(node))
        widget = new TickNodeWidget2D(type, tk, this, worldPosition);

    else if (auto* cn = dynamic_cast<CompareNode*>(node)) {
        widget = new CompareNodeWidget2D(type, cn, this, worldPosition);
    }
    else if (auto* mn = dynamic_cast<MapperNode*>(node)) {
        widget = new MapperNodeWidget2D(type, mn, this, worldPosition);
    }

    else
        widget = new NodeWidget2D(type, node, this, worldPosition);


    if (graph_container) {
        graph_container->add_child(widget);
        
    }
    else if (auto* eng = Engine::get_instance(); eng && eng->get_main_scene()) {
        eng->get_main_scene()->add_node(widget);
    }
    else {
        std::cout << "[GraphEditor] graph_container is null!\n";
    }
    
    
    widgets.push_back(widget);
    return node;
}

void GraphEditor::beginConnection(GraphNode* sourceNode,
    const std::string& outputPort)
{
    pendingSource = sourceNode;
    pendingOutput = outputPort;
}

void GraphEditor::completeConnection(GraphNode* targetNode,
    const std::string& inputPort)
{
    if (!pendingSource) return;

    if (!graph->connect(pendingSource, pendingOutput, targetNode, inputPort)) {
        std::cerr << "[GraphEditor] Failed to connect "
            << pendingSource->getName() << " -> "
            << targetNode->getName() << "\n";
    }
    else {
        std::cout << "[GraphEditor] Connected "
            << pendingSource->getName() << " -> "
            << targetNode->getName() << "\n";
    }

    resetConnectionState();
}

void GraphEditor::resetConnectionState()
{
    pendingSource = nullptr;
    pendingOutput.clear();
}

void GraphEditor::update(float delta_time)
{

    if (visualLink) visualLink->update(delta_time);

}

void GraphEditor::serialize(const std::string& path) {
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file) {
        spdlog::error("GraphEditor: Could not open file for writing {}", path);
        return;
    }

    // 1. Cabecera del grafo
    sGraphBinaryHeader header = { (uint64_t)getWidgets().size(), (uint64_t)graph->getLinks().size() };
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));

    uint64_t graph_name_size = graph->getName().size();
    file.write(reinterpret_cast<const char*>(&graph_name_size), sizeof(uint64_t));
    file.write(graph->getName().c_str(), graph_name_size);

    // 2. Serializar el almacén de variables
    const auto& varStore = VariableNode::getStore();
    uint64_t var_count = varStore.size();
    file.write(reinterpret_cast<const char*>(&var_count), sizeof(var_count));
    for (const auto& pair : varStore) {
        uint64_t name_size = pair.first.size();
        file.write(reinterpret_cast<const char*>(&name_size), sizeof(name_size));
        file.write(pair.first.c_str(), name_size);
        serializeVariableValue(file, pair.second);
    }

    // 3. Serializar todos los nodos
    for (auto* node_widget : getWidgets()) {
        node_widget->serialize(file);
    }

    // 4. Serializar todos los links
    for (const auto* link : graph->getLinks()) {
        GraphNode* source_node = link->getSourceNode();
        Output* output_pin = link->getOutput();
        GraphNode* target_node = link->getTargetNode();
        Input* input_pin = link->getTargetInput();
        if (!source_node || !output_pin || !target_node || !input_pin) {
            spdlog::warn("Skipping serialization of an invalid link.");
            continue;
        }
        uint64_t source_name_size = source_node->getName().size();
        file.write(reinterpret_cast<const char*>(&source_name_size), sizeof(uint64_t));
        file.write(source_node->getName().c_str(), source_name_size);

        uint64_t output_port_size = output_pin->getName().size();
        file.write(reinterpret_cast<const char*>(&output_port_size), sizeof(uint64_t));
        file.write(output_pin->getName().c_str(), output_port_size);

        uint64_t target_name_size = target_node->getName().size();
        file.write(reinterpret_cast<const char*>(&target_name_size), sizeof(uint64_t));
        file.write(target_node->getName().c_str(), target_name_size);

        uint64_t input_port_size = input_pin->getName().size();
        file.write(reinterpret_cast<const char*>(&input_port_size), sizeof(uint64_t));
        file.write(input_pin->getName().c_str(), input_port_size);
    }
    file.close();
}

void GraphEditor::parse(const std::string& path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        spdlog::error("GraphEditor: Could not parse file {}", path);
        return;
    }

    graph->clear();
    for (auto* w : widgets) { delete w; }
    widgets.clear();
    VariableNode::clearStore();

    sGraphBinaryHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    uint64_t graph_name_size = 0;
    file.read(reinterpret_cast<char*>(&graph_name_size), sizeof(uint64_t));
    std::string graph_name(graph_name_size, '\0');
    if (graph_name_size > 0) file.read(&graph_name[0], graph_name_size);
    graph->setName(graph_name);

    // PASO 0: Leer y repoblar el almacén de variables
    uint64_t var_count = 0;
    file.read(reinterpret_cast<char*>(&var_count), sizeof(var_count));
    for (uint64_t i = 0; i < var_count; ++i) {
        uint64_t name_size = 0;
        file.read(reinterpret_cast<char*>(&name_size), sizeof(name_size));
        std::string var_name(name_size, '\0');
        if (name_size > 0) file.read(&var_name[0], name_size);

        VariableValue var_value = parseVariableValue(file);
        VariableNode::setStoredValue(var_name, var_value);
    }

    // PASO 1: Cargar todos los nodos
    for (uint64_t i = 0; i < header.node_count; ++i) {
        uint64_t type_size = 0;
        file.read(reinterpret_cast<char*>(&type_size), sizeof(type_size));
        std::string node_type(type_size, '\0');
        if (type_size > 0) file.read(&node_type[0], type_size);

        GraphNode* new_node = createNode(node_type, "TEMP_LOADING");
        if (!new_node) continue;

        NodeWidget2D* new_widget = widgets.back();

        new_widget->parse(file);
        new_node->rebindPins();
        new_widget->updateTitleFromLogicNode();
        new_widget->rebuildWidgetUI();
        new_widget->updateInspector();
    }

    // PASO 2: Reconectar todos los links
    for (uint64_t i = 0; i < header.link_count; ++i) {
        uint64_t source_name_size, output_port_size, target_name_size, input_port_size;
        std::string source_node_name, output_port_name, target_node_name, input_port_name;

        file.read(reinterpret_cast<char*>(&source_name_size), sizeof(uint64_t));
        source_node_name.resize(source_name_size);
        file.read(&source_node_name[0], source_name_size);

        file.read(reinterpret_cast<char*>(&output_port_size), sizeof(uint64_t));
        output_port_name.resize(output_port_size);
        file.read(&output_port_name[0], output_port_size);

        file.read(reinterpret_cast<char*>(&target_name_size), sizeof(uint64_t));
        target_node_name.resize(target_name_size);
        file.read(&target_node_name[0], target_name_size);

        file.read(reinterpret_cast<char*>(&input_port_size), sizeof(uint64_t));
        input_port_name.resize(input_port_size);
        file.read(&input_port_name[0], input_port_size);

        GraphNode* source_node = graph->getNodeByName(source_node_name);
        GraphNode* target_node = graph->getNodeByName(target_node_name);

        if (source_node && target_node) {
            graph->connect(source_node, output_port_name, target_node, input_port_name);
        }
        else {
            spdlog::error("Link parse error: Node not found. {} -> {}", source_node_name, target_node_name);
        }
    }

    // PASO 3: Post-Procesamiento para enlazar entidades de la escena
    Scene* scene = Engine::get_instance()->get_main_scene();
    if (scene) {
        for (auto* widget : widgets) {
            if (auto* entityNode = dynamic_cast<EntityNode3D*>(widget->getLogicNode())) {
                const std::string& entity_name_to_find = entityNode->getEntityNameOnLoad();
                if (!entity_name_to_find.empty()) {

                    // FIX: Iterar sobre la lista de nodos de la escena para encontrar por nombre
                    Node* found_node = nullptr;
                    for (Node* scene_node : scene->get_nodes()) {
                        if (scene_node->get_name() == entity_name_to_find) {
                            found_node = scene_node;
                            break;
                        }
                    }

                    if (auto* meshInstance = dynamic_cast<MeshInstance3D*>(found_node)) {
                        entityNode->setEntity(meshInstance);
                    }
                    else {
                        spdlog::warn("Could not find or cast scene entity '{}' for node '{}'", entity_name_to_find, entityNode->getName());
                    }
                }
            }
        }
    }
    file.close();
}

