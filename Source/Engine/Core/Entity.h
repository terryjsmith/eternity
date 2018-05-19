
#ifndef entity_h
#define entity_h

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/Component.h>
#include <Core/Application.h>
#include <Core/ComponentSystem.h>
#include <Core/World.h>

GIGA_CLASS()
class GIGA_API Entity : public GigaObject {
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
    GIGA_FUNCTION() std::string Name() { return entityName; }
    void Name(std::string name) { entityName = name; }
    
    /**
     * Find component by name
     */
    GIGA_FUNCTION() Component* FindComponent(std::string name);

	/**
	* Assign a new component
	*/
    template<class T> T* Assign() {
        // Register with appropriate system
        World* world = Application::GetInstance()->GetWorld();
        ComponentSystem<T>* system = world->GetComponentSystem<T>();
        
        T* component = 0;
        if(system) {
            component = system->CreateComponent();
        }
        else {
            component = new T;
        }
        
        component->m_parent = this;
        m_components.push_back(component);
        
        // Return
        return(component);
    }

	friend class World;

public:
	// Identifier (string)
	std::string entityName;

protected:
	// Identifer (int)
	int m_entityID;

	// Components assigned to this entity
	std::vector<Component*> m_components;

	// Active status
	bool m_active;

	// Updated in the last frame
	bool m_updated;
};

#endif
