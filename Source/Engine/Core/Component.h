
#ifndef component_h
#define component_h

#include <eternity.h>
#include <Core/GigaObject.h>

class Entity;

class GIGA_API Component : public GigaObject {
public:
    Component();
	virtual ~Component() = default;
    
    // Component type defintion
    typedef Component* (*ComponentCreateFunc)();
    
    struct ComponentType {
        std::string name;
        uint32_t typeID;
        ComponentCreateFunc ctor;
    };
    
    /**
     * Register a new component type
     */
    template<class T>
    static void RegisterComponentType(std::string name, int typeID) {
        std::map<std::string, ComponentType*>::iterator it = m_types.find(name);
        GIGA_ASSERT(it == m_types.end(), "Component type already registered.");
        
        ComponentType* ct = new ComponentType();
        ct->name = name;
        ct->typeID = typeID;
        ct->ctor = InternalCreateComponent<T>;
        
        m_types[name] = ct;
    }
    
    /**
     * Create a component from a name
     */
    static Component* CreateComponent(std::string className);
    
    /**
     * Get type ID (lazy)
     */
    uint32_t GetTypeID();
    
    /**
     * Get parent entity
     */
    Entity* GetParent() { return m_parent; }
    
    friend class Entity;
    
protected:
    // Create a new component from a class type
    template<typename T> static Component* InternalCreateComponent() { return new T; }

protected:
	// What type of component are we?
	uint32_t m_typeID;

	// Parent entity
	Entity* m_parent;
    
    // Registered type IDs
    static std::map<std::string, ComponentType*> m_types;
};

#endif
