#ifndef IO_H
#define IO_H

#include <string>
#include <list>
#include <glm/glm.hpp>
#include "framework/nodes/mesh_instance_3d.h"
#include <functional> 
#include <iostream>

namespace GraphSystem {

    // Forward declarations
    class Link;
    class GraphNode;
    class Output; // Necesario para la referencia en IO

    enum class IOType {
        BOOL, INT, FLOAT, STRING,
        VEC2, VEC3, VEC4, MAT4,
        TRANSFORM, MESH, EXECUTION
    };

    inline bool isLazyType(IOType type) {
        switch (type) {
        case IOType::FLOAT:
        case IOType::INT:
        case IOType::VEC2:
        case IOType::VEC3:
        case IOType::VEC4:
            return true;
        default:
            return false;
        }
    }

    class IO {
    protected:
        std::string name;
        IOType type;
        bool is_dirty;

        Output* connectedOutput = nullptr; 

        union Data {
            bool boolValue;
            int intValue;
            float floatValue;
            std::string stringValue;
            glm::vec2 vec2Value;
            glm::vec3 vec3Value;
            glm::vec4 vec4Value;
            glm::mat4 mat4Value;
            Transform transformValue;
            MeshInstance3D* meshValue;

            Data() : stringValue() {}
            ~Data() {}
        } data;

    public:
        IO(const std::string& name, IOType type);
        virtual ~IO();

        IO(const IO&) = delete;
        IO& operator=(const IO&) = delete;

        void connectTo(Output* output) { connectedOutput = output; }
        Output* getConnectedOutput() const { return connectedOutput; }

        template<typename T>
        void setData(const T& value);

        void setData(MeshInstance3D* ptr);

        void copyTo(IO* dst) const;

        // Type-specific getters
        bool getBool() const;
        int getInt() const;
        float getFloat() const;
        std::string getString() const;
        glm::vec2 getVec2() const;
        glm::vec3 getVec3() const;
        glm::vec4 getVec4() const;
        glm::mat4 getMat4() const;
        Transform getTransform() const;
        MeshInstance3D* getMesh() const;

        IOType getType() const { return type; }
        const std::string& getName() const { return name; }
        bool hasData() const;
        bool isDirty() const { return is_dirty; }
        void markClean() { is_dirty = false; }
    };

    class Input : public IO {
    public:
        Input(const std::string& name, IOType type);
        ~Input() = default;
    };

    class Output : public IO {
    private:
        GraphNode* ownerNode;
        std::list<Link*> links;

        std::function<float()> computeFloatFunction;
        std::function<MeshInstance3D*()> computeMeshFunction;


    public:
        Output(GraphNode* owner, const std::string& name, IOType type);
        ~Output();

        GraphNode* getOwnerNode() const { return ownerNode; }

        void addLink(Link* link);
        void removeLink(Link* link);

        std::list<Link*>& getLinks();
        const std::list<Link*>& getLinks() const;

        void setComputeFunction(std::function<float()> func) {
            if (!isLazyType(this->getType())) {
                throw std::runtime_error("[Output] Trying to set computeFunction on non-lazy IOType: " + std::to_string(static_cast<int>(type)));
            }

            std::cout << "[Output] Setting computeFunction on " << name << " Type: " << static_cast<int>(type) << "\n";
            computeFloatFunction = func;
        }

        void setComputeFunction(std::function<MeshInstance3D* ()> func) {
            if (this->getType() != IOType::MESH) {
                throw std::runtime_error("[Output] Trying to set computeFunction on non-MESH type: " + std::to_string(static_cast<int>(type)));
            }
            computeMeshFunction = func;
        }

        std::function<float()> getComputeFunction() const {
            return computeFloatFunction;
        }

        std::function<MeshInstance3D*()> getComputeMeshFunction() const {
            return computeMeshFunction;
        }

        Output(const Output&) = delete;
        Output& operator=(const Output&) = delete;
    };

} // namespace GraphSystem

#endif // IO_H
