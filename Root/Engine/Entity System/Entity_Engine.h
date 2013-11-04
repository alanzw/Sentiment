
#ifndef ENTITY_ENGINE_H
#define ENTITY_ENGINE_H

#include "Root/Utility/Intrusive/Intrusive_list.h"
#include "Root/Utility/Intrusive/Intrusive_map.h"
#include <string>
#include <vector>

class World;

class base_node :
    public intrusive::dynamic_node
{
public:
    virtual long GetTypeID() const = 0;

    virtual long GetUniqueID() const = 0;

    virtual const std::string& GetUniqueName() const = 0;

private:
    static long m_nextTypeID;
    static long m_nextUniqueID;
};



template<class TDerived>
class node :
    public base_node
{
public:
    node(const std::string& uniqueName) :
        m_uniqueID(m_nextUniqueID++),
        m_uniqueName(uniqueName)
        {}

    long GetTypeID() const final {return m_typeID;}

    long GetUniqueID() const final {return m_uniqueID;}

    const std::string& GetUniqueName() const final {return m_uniqueName;}

private:
    static const long m_typeID;
    const long m_uniqueID;
    const std::string m_uniqueName;
};


class base_component :
    public intrusive::dynamic_node
{
public:
    virtual long GetTypeID() const = 0;

    virtual long GetUniqueID() const = 0;

    virtual const std::string& GetUniqueName() const = 0;

private:
    static long m_nextTypeID;
    static long m_nextUniqueID;
    intrusive::list<base_node> m_DependantNodes;
};


template<class TDerived>
class component :
    public base_component
{
public:
    component(const std::string& uniqueName, const std::string& entityName) :
        m_uniqueID(m_nextUniqueID++),
        m_uniqueName(uniqueName)
        {}

    long GetTypeID() const final {return m_typeID;}

    long GetUniqueID() const final {return m_uniqueID;}

    const std::string& GetUniqueName() const = 0;

private:
    static const long m_typeID;
    const long m_uniqueID;
    const std::string m_uniqueName;
};


class Entity
{
protected:


private:
    World * m_World;
    intrusive::map<std::string, base_node> m_Nodes;
    intrusive::map<std::string, base_component> m_Components;
};



class World
{
public:
    void CreateEntity(const std::string& name);

    template<class T, class ... Args>
    void CreateComponent(const std::string& name, const std::string& entity, const Args&... params);

    template<class T, class ... Args>
    void CreateNode(const std::string& name, const std::string& entity, const Args&... params);

    template<class T, class ... Args>
    void CreateSystem(const std::string& name);

    void RemoveComponent(const std::string& name);

    void RemoveNode(const std::string& name);

    void RemoveEntity(const std::string& name);

    void RemoveSystem(const std::string& name);

private:
    std::map<long, std::vector<base_component *> > m_Components;
    std::map<long, std::vector<base_node *> > m_Nodes;
    intrusive::map<std::string, base_component> m_ComponentByName;
    intrusive::map<std::string, base_node> m_NodeByName;
    std::map<std::string, base_entity> m_EntityList;
};



long base_node::m_nextTypeID = 0;
long base_node::m_nextUniqueID = 0;
template<class TDerived>
const long node<TDerived>::m_typeID = m_nextTypeID++;
long base_component::m_nextTypeID = 0;
long base_component::m_nextUniqueID = 0;
template<class TDerived>
const long component<TDerived>::m_typeID = m_nextTypeID++;


#endif
