#ifndef IO_H
#define IO_H

#include <string>
#include <list>
#include <variant>
#include <functional>
#include <glm/glm.hpp>
#include "framework/nodes/mesh_instance_3d.h"

namespace GraphSystem {

    class Link;
    class GraphNode;


    enum class IOType {
        BOOL, INT, FLOAT, STRING,
        VEC2, VEC3, VEC4, MAT4,
        MESH, EXECUTION
    };

    using VariableValue = std::variant<
        bool, int, float, std::string,
        glm::vec2, glm::vec3, glm::vec4, glm::mat4,
        MeshInstance3D*
    >;

    class Output; 

    class IO {
    protected:
        std::string name;
        IOType type;
        bool is_dirty;
        Output* connectedOutput; 
        GraphSystem::VariableValue value; 

      
    public:
        IO(const std::string& name, IOType type);
        virtual ~IO() = default;

        IO(const IO&) = delete;
        IO& operator=(const IO&) = delete;

        void connectTo(Output* output) { connectedOutput = output; }
        Output* getConnectedOutput() const { return connectedOutput; }

        virtual void setData(const GraphSystem::VariableValue& v);

        const VariableValue& getRawValue() const { return value; }

        virtual bool getBool() const;
        virtual int getInt() const;
        virtual float getFloat() const;
        virtual std::string getString() const;
        virtual glm::vec2 getVec2() const;
        virtual glm::vec3 getVec3() const;
        virtual glm::vec4 getVec4() const;
        virtual glm::mat4 getMat4() const;
        virtual MeshInstance3D* getMesh() const;

        IOType getType() const { return type; }
        const std::string& getName() const { return name; }
        virtual bool hasData() const; 
        bool isDirty() const { return is_dirty; }
        void markClean() { is_dirty = false; }

   
    };

    class Input : public IO {
    public:
        Input(const std::string& name, IOType type);

        VariableValue getValue() const;

    };

    class Output : public IO {
    private:
        GraphNode* ownerNode;
        std::list<Link*> links;
        std::function<GraphSystem::VariableValue()> computeFunction; 

    public:
        Output(GraphNode* owner, const std::string& name, IOType type);
        ~Output() override;

        GraphNode* getOwnerNode() const { return ownerNode; }

        void addLink(Link* link);
        void removeLink(Link* link);
        std::list<Link*>& getLinks();
        const std::list<Link*>& getLinks() const;

        void setComputeFunction(std::function<GraphSystem::VariableValue()> func) { computeFunction = func; }
        std::function<GraphSystem::VariableValue()> getComputeFunction() const { return computeFunction; }

        bool getBool() const override;
        int getInt() const override;
        float getFloat() const override;
        std::string getString() const override;
        glm::vec2 getVec2() const override;
        glm::vec3 getVec3() const override;
        glm::vec4 getVec4() const override;
        glm::mat4 getMat4() const override;
        MeshInstance3D* getMesh() const override;

        void setData(const GraphSystem::VariableValue& v) override;
        bool hasData() const override;

        Output(const Output&) = delete;
        Output& operator=(const Output&) = delete;
    };

} 
#endif 
