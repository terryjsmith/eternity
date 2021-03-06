
#ifndef entity_h
#define entity_h

#include <eternity.h>
#include <Core/DataObject.h>
#include <Core/GigaObject.h>
#include <Core/Component.h>
#include <Core/Application.h>
#include <Core/ComponentSystem.h>
#include <Core/World.h>

GIGA_CLASS()
class GIGA_API Entity : public DataObject {
public:
    Entity();
	~Entity() = default;
    
    GIGA_CLASS_NAME("Entity");

	/**
	 * Get/set active status
	 */
	bool IsActive() { return m_active; }
	void IsActive(bool active);

	/**
	 * Get/set updated status
	 */
	bool IsUpdated() { return m_updated; }
	void IsUpdated(bool updated);

	/**
	* Get/set entity ID
	*/
	GIGA_FUNCTION() int ID() { return m_entityID; }
	void ID(int id);
    
    /**
     * Get/set name
     */
    GIGA_FUNCTION() std::string Name() { return m_entityName; }
    void Name(std::string name) { m_entityName = name; }
    
    /**
     * Find component by name
     */
    GIGA_FUNCTION() Component* FindComponent(std::string name);
    Component* FindComponent(uint32_t typeID);
    
    /**
     * Add component
     */
    void AddComponent(Component* component, bool notify = true);
    
    /**
     * Find a component by class type
     */
    template<class T> T* FindComponent() {
        std::vector<Component*>::iterator ci = m_components.begin();
        for(; ci != m_components.end(); ci++) {
            T* obj = dynamic_cast<T*>(*ci);
            if(obj) {
                return(obj);
            }
        }
        
        return(0);
    }

	/**
	* Assign a new component
	*/
    template<class T> T* Assign() {
        // Register with appropriate system
        World* world = Application::GetInstance()->GetWorld();
        std::vector<ComponentSystemBase*> systems = world->GetComponentSystems();
        std::vector<ComponentSystemBase*>::iterator it = systems.begin();
        
        T* component = new T;
        for(; it != systems.end(); it++) {
            (*it)->AddComponent(component);
        }
        
        // Send message to other components
        std::vector<Component*>::iterator ci = m_components.begin();
        for(; ci != m_components.end(); ci++) {
            (*ci)->OnComponentAdded(component);
        }
        
        component->SetParent(this);
        component->OnEntityAssigned();
        m_components.push_back(component);
        
        // Return
        return(component);
    }
    
    GIGA_FUNCTION() Component* Assign(std::string className);
    
    /**
     * Get all components
     */
    std::vector<Component*>& GetComponents() { return m_components; }
    
    /**
     * Serialization
     */
    void Deserialize(DataRecord* record);
    void Serialize(DataRecord* record);

	friend class World;

protected:
	// Identifier (string)
	std::string m_entityName;

	// Identifer (int)
	int m_entityID;

	// Components assigned to this entity
	std::vector<Component*> m_components;

	// Active status
	bool m_active;

	// Updated in the last frame
	bool m_updated;

	// Which scene ID is this entity in?
	int m_sceneID;
};

#endif
