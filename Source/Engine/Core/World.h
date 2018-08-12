
#ifndef world_h
#define world_h

#include <eternity.h>
#include <Core/ComponentSystem.h>

class Entity;

/**
* ECS World - contains all entities, components, and systems
*/
GIGA_CLASS()
class GIGA_API World : public GigaObject {
public:
	World() = default;
	~World() = default;
    
    GIGA_CLASS_NAME("World");

	/**
	 * Initialize the world (systems)
	 */
	void Initialize();

	/**
	 * Shutdown the world (systems)
	 */
	void Shutdown();

	/**
	 * Update systems
	 */
	void Update(float delta);

	/**
	* Create a new entity
	*/
	GIGA_FUNCTION() Entity* CreateEntity();

	/**
	* Find an entity by ID
	*/
	GIGA_FUNCTION() Entity* FindEntity(int entityID);
    
    /**
     * Add an entity
     */
    void AddEntity(Entity* entity);

	/**
	 * Clear out all entities
	 */
	void Clear();

	/**
	 * Registered system with update tick rate
	 */
	struct RegisteredSystem {
		int tickRate;
		float accumulator;
		System* system;
	};

	/**
	* Register a new subsystem
	*/
	template<class T>
	T* CreateSystem(int tickRate = 0) {
		T* obj = new T();
		System* sys = dynamic_cast<System*>(obj);
		GIGA_ASSERT(sys != 0, "Class must be inherited from System type.");

		RegisteredSystem* rs = new RegisteredSystem();
		rs->tickRate = tickRate;
		rs->system = sys;
		rs->accumulator = 0;
		m_systems.push_back(rs);

		return(obj);
	}

	/**
	* Find a specific subsystem by class type
	*/
	template<class T>
	T* GetSystem() {
		std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
		for (; i != m_systems.end(); i++) {
			T* object = dynamic_cast<T*>((*i)->system);
			if (object) {
				return(object);
			}
		}

		return(0);
	}

	/**
	* Get systems
	*/
    std::vector<ComponentSystemBase*> GetComponentSystems() {
        std::vector<ComponentSystemBase*> systems;
        std::vector<RegisteredSystem*>::iterator it = m_systems.begin();
        for(; it != m_systems.end(); it++) {
            ComponentSystemBase* base = dynamic_cast<ComponentSystemBase*>((*it)->system);
            if(base) {
                systems.push_back(base);
            }
        }
        
        return(systems);
	}
    
    /**
     * Find a system by name
     */
    GIGA_FUNCTION() System* FindSystem(std::string name);
    
    /**
     * Get all entities
     */
    std::vector<Entity*>& GetEntities() { return m_entities; }

protected:
	// Known system types
	std::vector<RegisteredSystem*> m_systems;

	// Known entities
	std::vector<Entity*> m_entities;

	// Entity ID counter
	int m_entityCounter;
};

#endif
