
#include <Core/World.h>
#include <Core/Entity.h>

void World::Initialize() {
	m_entityCounter = 0;

	// Initialize all systems
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		System* sys = (*i)->system;
		sys->Initialize();
	}
}

void World::Shutdown() {
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		System* sys = (*i)->system;
		sys->Shutdown();
	}
}

void World::Update(float delta) {
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();

	for (; i != m_systems.end(); i++) {
		if ((*i)->tickRate > 0) {
			(*i)->accumulator += delta;

			if ((*i)->accumulator > (1.0f / (*i)->tickRate)) {
				float theta = (1.0f / (*i)->tickRate);
				(*i)->accumulator -= theta;
				(*i)->system->Update(theta);
			}
		}
	}

	i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		if ((*i)->tickRate == 0) {
			(*i)->system->Update(delta);
		}
	}
}

Entity* World::CreateEntity() {
	// Create a new entity
	Entity* entity = new Entity();
	entity->ID(++m_entityCounter);

	// Add it to the list
	m_entities.push_back(entity);
	return(m_entities.back());
}

Entity* World::GetEntity(int id) {
	std::vector<Entity*>::iterator it = m_entities.begin();
	for (; it != m_entities.end(); it++) {
		if ((*it)->m_entityID == id) {
			return((*it));
		}
	}

	return(0);
}
