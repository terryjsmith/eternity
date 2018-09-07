
#ifndef component_h
#define component_h

#include <eternity.h>
#include <Core/DataObject.h>
#include <Core/GigaObject.h>

class Entity;

class GIGA_API Component : public DataObject {
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
     * Create a component from a type ID
     */
    static Component* CreateComponent(uint32_t type);

	/**
	 * Get registered component types
	 */
	static std::vector<std::string> GetComponentTypes();
    
    /**
     * Get/set updated status
     */
    bool Updated() { return m_updated; }
    void Updated(bool updated);
    
    /**
     * Set active status
     */
    bool Active();
    void Active(bool active) { m_active = active; }
    
    
    /**
     * Get type ID (lazy)
     */
    uint32_t GetTypeID();
    
    /**
     * Get/set parent entity
     */
    void SetParent(Entity* entity) { m_parent = entity; }
    Entity* GetParent() { return m_parent; }
    
    /**
     * Overridable callbacks
     */
    
    // Called on existing components when a new component is added/removed
    virtual void OnComponentAdded(Component* component) { }
    virtual void OnComponentRemoved(Component* component) { }
    
    // Called on the new component when it is added/removed
    virtual void OnEntityAssigned() { }
    virtual void OnEntityUnassigned() { }
    
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

	// Which scene ID is this component in?
	int m_sceneID;
    
    // Whether this component has been updated or not
    bool m_updated;
    
    // Whether this component is active or not
     bool m_active;
};

#endif
