
#ifndef world_h
#define world_h

#include <eternity.h>
#include <Core/ComponentSystem.h>

class Entity;

/**
* ECS World - contains all entities, components, and systems
*/
class GIGA_API World {
public:
	World() = default;
	~World() = default;

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
	Entity* CreateEntity();

	/**
	* Find an entity by ID
	*/
	Entity* GetEntity(int id);

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
	template<class T>
	ComponentSystem<T>* GetComponentSystem() {
		std::vector<RegisteredSystem*>::iterator it = m_systems.begin();
		for (; it != m_systems.end(); it++) {
			ComponentSystem<T>* system = dynamic_cast<ComponentSystem<T>*>((*it)->system);
			if (system) {
				return(system);
			}
		}

		return(0);
	}

protected:
	// Known system types
	std::vector<RegisteredSystem*> m_systems;

	// Known entities
	std::vector<Entity*> m_entities;

	// Entity ID counter
	int m_entityCounter;
};

#endif
