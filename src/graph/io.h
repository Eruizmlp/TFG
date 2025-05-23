#ifndef IO_H
#define IO_H

#include <string>
#include <list>
#include <glm/glm.hpp>
#include "framework/nodes/node_3d.h"

namespace GraphSystem {

    // Forward declarations
    class Link;
    class GraphNode;

    enum class IOType {
        BOOL, INT, FLOAT, STRING,
        VEC2, VEC3, VEC4, MAT4,
        TRANSFORM, EXECUTION
    };

    class IO {
    protected:
        std::string name;
        IOType type;
        bool is_dirty;

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

            Data() : stringValue() {}
            ~Data() {}
        } data;

    public:
        IO(const std::string& name, IOType type);
        virtual ~IO();

        IO(const IO&) = delete;
        IO& operator=(const IO&) = delete;

        // Template method for type-safe data setting
        template<typename T>
        void setData(const T& value);

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
    public:
        Output(GraphNode* owner, const std::string& name, IOType type);
        ~Output();

        GraphNode* getOwnerNode() const { return ownerNode; }

        void addLink(Link* link);
        void removeLink(Link* link);

        std::list<Link*>& getLinks() { return links; }
        const std::list<Link*>& getLinks() const { return links; }

        Output(const Output&) = delete;
        Output& operator=(const Output&) = delete;
    };

} // namespace GraphSystem

#endif // IO_H
