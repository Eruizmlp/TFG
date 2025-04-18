#include "io.h"
#include "link.h"
#include <stdexcept>
#include <iostream>

namespace GraphSystem {

    IO::IO(const std::string& name, IOType type)
        : name(name), type(type), is_dirty(false) {
        switch (type) {
        case IOType::BOOL: data.boolValue = false; break;
        case IOType::INT: data.intValue = 0; break;
        case IOType::FLOAT: data.floatValue = 0.0f; break;
        case IOType::STRING: new (&data.stringValue) std::string(); break;
        case IOType::VEC2: data.vec2Value = glm::vec2(0.0f); break;
        case IOType::VEC3: data.vec3Value = glm::vec3(0.0f); break;
        case IOType::VEC4: data.vec4Value = glm::vec4(0.0f); break;
        case IOType::MAT4: data.mat4Value = glm::mat4(1.0f); break;
        case IOType::TRANSFORM: data.transformValue = Transform(); break;
        case IOType::EXECUTION: break;
        default: throw std::runtime_error("Unknown IOType in IO constructor");
        }
    }

    IO::~IO() {
        if (type == IOType::STRING) {
            data.stringValue.~basic_string();
        }
    }

    // Template specialization implementation
    template<typename T>
    void IO::setData(const T& value) {
        throw std::runtime_error("Unsupported data type for IO");
    }

    // Explicit template specializations
    template<> void IO::setData<bool>(const bool& value) {
        if (type != IOType::BOOL) throw std::runtime_error("Bool data type mismatch");
        data.boolValue = value;
        is_dirty = true;
    }

    template<> void IO::setData<int>(const int& value) {
        if (type != IOType::INT) throw std::runtime_error("Int data type mismatch");
        data.intValue = value;
        is_dirty = true;
    }

    template<> void IO::setData<float>(const float& value) {
        if (type != IOType::FLOAT) throw std::runtime_error("Float data type mismatch");
        data.floatValue = value;
        is_dirty = true;
    }

    template<> void IO::setData<std::string>(const std::string& value) {
        if (type != IOType::STRING) throw std::runtime_error("String data type mismatch");
        data.stringValue = value;
        is_dirty = true;
    }

    template<> void IO::setData<glm::vec2>(const glm::vec2& value) {
        if (type != IOType::VEC2) throw std::runtime_error("Vec2 data type mismatch");
        data.vec2Value = value;
        is_dirty = true;
    }

    template<> void IO::setData<glm::vec3>(const glm::vec3& value) {
        if (type != IOType::VEC3) throw std::runtime_error("Vec3 data type mismatch");
        data.vec3Value = value;
        is_dirty = true;
    }

    template<> void IO::setData<glm::vec4>(const glm::vec4& value) {
        if (type != IOType::VEC4) throw std::runtime_error("Vec4 data type mismatch");
        data.vec4Value = value;
        is_dirty = true;
    }

    template<> void IO::setData<glm::mat4>(const glm::mat4& value) {
        if (type != IOType::MAT4) throw std::runtime_error("Mat4 data type mismatch");
        data.mat4Value = value;
        is_dirty = true;
    }

    template<> void IO::setData<Transform>(const Transform& value) {
        if (type != IOType::TRANSFORM) throw std::runtime_error("Transform data type mismatch");
        data.transformValue = value;
        is_dirty = true;
    }

    // Data getters
    bool IO::getBool() const {
        if (type != IOType::BOOL) throw std::runtime_error("Bool data type mismatch");
        return data.boolValue;
    }

    int IO::getInt() const {
        if (type != IOType::INT) throw std::runtime_error("Int data type mismatch");
        return data.intValue;
    }

    float IO::getFloat() const {
        if (type != IOType::FLOAT) throw std::runtime_error("Float data type mismatch");
        return data.floatValue;
    }

    std::string IO::getString() const {
        if (type != IOType::STRING) throw std::runtime_error("String data type mismatch");
        return data.stringValue;
    }

    glm::vec2 IO::getVec2() const {
        if (type != IOType::VEC2) throw std::runtime_error("Vec2 data type mismatch");
        return data.vec2Value;
    }

    glm::vec3 IO::getVec3() const {
        if (type != IOType::VEC3) throw std::runtime_error("Vec3 data type mismatch");
        return data.vec3Value;
    }

    glm::vec4 IO::getVec4() const {
        if (type != IOType::VEC4) throw std::runtime_error("Vec4 data type mismatch");
        return data.vec4Value;
    }

    glm::mat4 IO::getMat4() const {
        if (type != IOType::MAT4) throw std::runtime_error("Mat4 data type mismatch");
        return data.mat4Value;
    }

    Transform IO::getTransform() const {
        if (type != IOType::TRANSFORM) throw std::runtime_error("Transform data type mismatch");
        return data.transformValue;
    }

    bool IO::hasData() const {
        switch (type) {
        case IOType::BOOL: return true;
        case IOType::INT: return true;
        case IOType::FLOAT: return true;
        case IOType::STRING: return !data.stringValue.empty();
        case IOType::VEC2: return true;
        case IOType::VEC3: return true;
        case IOType::VEC4: return true;
        case IOType::MAT4: return true;
        case IOType::TRANSFORM: return true;
        case IOType::EXECUTION: return false;
        default: return false;
        }
    }

    // Input implementation
    Input::Input(const std::string& name, IOType type) : IO(name, type) {}

    // Output implementation
    Output::Output(GraphNode* owner, const std::string& name, IOType type)
        : IO(name, type), ownerNode(owner) {} 

    Output::~Output() {
        // Clean up all links
        for (auto* link : links) {
            if (link) {
                delete link;
            }
        }
        links.clear();
    }

    void Output::addLink(Link* link) {
        if (link) {
            links.push_back(link);
        }
    }

    void Output::removeLink(Link* link) {
        if (link) {
            links.remove(link);
            delete link;  
        }
    }

} 
