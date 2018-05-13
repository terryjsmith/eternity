
#ifndef entity_h
#define entity_h

#include <eternity.h>
#include <Core/Component.h>

class GIGA_API Entity {
public:
	~Entity() = default;

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
	int ID() { return m_entityID; }
	void ID(int id);

	/**
	* Assign a new component
	*/
	template<class T> Component* Assign();

	friend class World;

public:
	// Identifier (string)
	std::string entityName;

protected:
	// Identifer (int)
	int m_entityID;

	// Protected constructor - should be created through World->CreateEntity()
	Entity();

	// Components assigned to this entity
	std::vector<Component*> m_components;

	// Active status
	bool m_active;

	// Updated in the last frame
	bool m_updated;
};

#endif