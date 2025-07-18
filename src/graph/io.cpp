#include "io.h"
#include "link.h"
#include "graph_node.h" 
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

namespace GraphSystem {

    IO::IO(const std::string& name, IOType type)
        : name(name), type(type), is_dirty(false), connectedOutput(nullptr) {
        switch (type) {
        case IOType::BOOL:   value = false; break;
        case IOType::INT:    value = 0; break;
        case IOType::FLOAT:  value = 0.0f; break;
        case IOType::STRING: value = std::string(); break;
        case IOType::VEC2:   value = glm::vec2(0.0f); break;
        case IOType::VEC3:   value = glm::vec3(0.0f); break;
        case IOType::VEC4:   value = glm::vec4(0.0f); break;
        case IOType::MAT4:   value = glm::mat4(1.0f); break;
        case IOType::MESH:   value = static_cast<MeshInstance3D*>(nullptr); break;
        case IOType::EXECUTION: break;
        default: throw std::runtime_error("Unknown IOType in IO constructor for " + name);
        }
    }

    void IO::setData(const VariableValue& v) {
        value = v;
        is_dirty = true;
    }

    bool IO::getBool() const {
        if (connectedOutput) { 
            return connectedOutput->getBool(); 
        }


        if (auto val = std::get_if<bool>(&value)) return *val;
        throw std::runtime_error("IO::getBool: Type mismatch or no data for bool. Name: " + name);
    }

    int IO::getInt() const {
        if (connectedOutput) {
            return connectedOutput->getInt();
        }
        if (auto val = std::get_if<int>(&value)) return *val;
        throw std::runtime_error("IO::getInt: Type mismatch or no data for int. Name: " + name);
    }

    float IO::getFloat() const {
        if (connectedOutput) {
            return connectedOutput->getFloat();
        }
        if (auto val = std::get_if<float>(&value)) return *val;
        throw std::runtime_error("IO::getFloat: Type mismatch or no data for float. Name: " + name);
    }

    std::string IO::getString() const {
        if (connectedOutput) {
            return connectedOutput->getString();
        }
        if (auto val = std::get_if<std::string>(&value)) return *val;
        throw std::runtime_error("IO::getString: Type mismatch or no data for string. Name: " + name);
    }

    glm::vec2 IO::getVec2() const {
        if (connectedOutput) {
            return connectedOutput->getVec2();
        }
        if (auto val = std::get_if<glm::vec2>(&value)) return *val;
        throw std::runtime_error("IO::getVec2: Type mismatch or no data for vec2. Name: " + name);
    }

    glm::vec3 IO::getVec3() const {
        if (connectedOutput) {
            return connectedOutput->getVec3();
        }
        if (auto val = std::get_if<glm::vec3>(&value)) return *val;
        throw std::runtime_error("IO::getVec3: Type mismatch or no data for vec3. Name: " + name);
    }

    glm::vec4 IO::getVec4() const {
        if (connectedOutput) {
            return connectedOutput->getVec4();
        }
        if (auto val = std::get_if<glm::vec4>(&value)) return *val;
        throw std::runtime_error("IO::getVec4: Type mismatch or no data for vec4. Name: " + name);
    }

    glm::mat4 IO::getMat4() const {
        if (connectedOutput) {
            return connectedOutput->getMat4();
        }
        if (auto val = std::get_if<glm::mat4>(&value)) return *val;
        throw std::runtime_error("IO::getMat4: Type mismatch or no data for mat4. Name: " + name);
    }

    MeshInstance3D* IO::getMesh() const {
        if (connectedOutput) {
            return connectedOutput->getMesh();
        }
        if (auto val = std::get_if<MeshInstance3D*>(&value)) return *val;

        return nullptr;
    }

    bool IO::hasData() const {
        if (connectedOutput) return connectedOutput->hasData();

        if (std::holds_alternative<MeshInstance3D*>(value)) {
            return std::get<MeshInstance3D*>(value) != nullptr;
        }
        return true; 
    }

    Input::Input(const std::string& name, IOType type) : IO(name, type) {}

    VariableValue Input::getValue() const {
        if (connectedOutput) {

            auto compute_func = connectedOutput->getComputeFunction();
            if (compute_func) {
                return compute_func();
            }
            // Si no, devuelve el valor que ya está almacenado en el pin 
            else {
                return connectedOutput->getRawValue();
            }
        }

        return value;
    }


    Output::Output(GraphNode* owner, const std::string& name, IOType type)
        : IO(name, type), ownerNode(owner), computeFunction(nullptr)
    {
     
    }

    Output::~Output() {

        links.clear();
    }

    void Output::addLink(Link* link) {
        if (link) links.push_back(link);
    }

    void Output::removeLink(Link* link) {
        if (link) links.remove(link);
    }

    std::list<Link*>& Output::getLinks() { return links; }
    const std::list<Link*>& Output::getLinks() const { return links; }

    bool Output::getBool() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<bool>(&cv)) return *vp;
            throw std::runtime_error("Output::getBool: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<bool>(&value)) return *vp; 
        throw std::runtime_error("Output::getBool: Type mismatch or no data. Name: " + getName());
    }

    int Output::getInt() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<int>(&cv)) return *vp;
            throw std::runtime_error("Output::getInt: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<int>(&value)) return *vp;
        throw std::runtime_error("Output::getInt: Type mismatch or no data. Name: " + getName());
    }

    float Output::getFloat() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<float>(&cv)) return *vp;
            throw std::runtime_error("Output::getFloat: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<float>(&value)) return *vp;
        throw std::runtime_error("Output::getFloat: Type mismatch or no data. Name: " + getName());
    }

    std::string Output::getString() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<std::string>(&cv)) return *vp;
            throw std::runtime_error("Output::getString: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<std::string>(&value)) return *vp;
        throw std::runtime_error("Output::getString: Type mismatch or no data. Name: " + getName());
    }

    glm::vec2 Output::getVec2() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<glm::vec2>(&cv)) return *vp;
            throw std::runtime_error("Output::getVec2: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<glm::vec2>(&value)) return *vp;
        throw std::runtime_error("Output::getVec2: Type mismatch or no data. Name: " + getName());
    }

    glm::vec3 Output::getVec3() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<glm::vec3>(&cv)) return *vp;
            throw std::runtime_error("Output::getVec3: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<glm::vec3>(&value)) return *vp;
        throw std::runtime_error("Output::getVec3: Type mismatch or no data. Name: " + getName());
    }

    glm::vec4 Output::getVec4() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<glm::vec4>(&cv)) return *vp;
            throw std::runtime_error("Output::getVec4: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<glm::vec4>(&value)) return *vp;
        throw std::runtime_error("Output::getVec4: Type mismatch or no data. Name: " + getName());
    }

    glm::mat4 Output::getMat4() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<glm::mat4>(&cv)) return *vp;
            throw std::runtime_error("Output::getMat4: Type mismatch from computeFunction. Name: " + getName());
        }
        if (auto vp = std::get_if<glm::mat4>(&value)) return *vp;
        throw std::runtime_error("Output::getMat4: Type mismatch or no data. Name: " + getName());
    }

    MeshInstance3D* Output::getMesh() const {
        if (computeFunction) {
            GraphSystem::VariableValue cv = computeFunction();
            if (auto vp = std::get_if<MeshInstance3D*>(&cv)) return *vp; 
            
            return nullptr; 
        }
        if (auto vp = std::get_if<MeshInstance3D*>(&value)) return *vp;
        return nullptr; 
    }

    void Output::setData(const GraphSystem::VariableValue& v) {
        
        IO::setData(v);
        
    }

    bool Output::hasData() const {
        if (computeFunction) return true; 
        
        if (std::holds_alternative<MeshInstance3D*>(value)) {
            return std::get<MeshInstance3D*>(value) != nullptr;
        }
        return true;
    }

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

        std::cout << "[DEBUG] parseVariableValue read type_index: " << static_cast<int>(type_index) << std::endl;

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

}
