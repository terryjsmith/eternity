
#include <Core/Entity.h>
#include <Core/Application.h>
#include <Core/ComponentSystem.h>
#include <Core/World.h>

Entity::Entity() {
	m_active = false;
	m_updated = false;
}

void Entity::IsActive(bool active) {

}

void Entity::IsUpdated(bool updated) {

}

void Entity::ID(int id) {
	// Make sure another entity does not exist with this ID
	World* world = Application::GetInstance()->GetWorld();

	Entity* e = world->GetEntity(id);
	assert(e == 0);

	m_entityID = id;
}

template<class T>
Component* Entity::Assign() {
	// Register with appropriate system
    World* world = Application::GetInstance()->GetWorld();
	ComponentSystem<T>* system = world->GetComponentSystem<T>();
	T* component = system->CreateComponent();

	// Return
	return(component);
}
