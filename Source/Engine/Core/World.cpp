
#include <Core/World.h>
#include <Core/Entity.h>
#include <Core/TransformComponent.h>

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
    
    // Assign the first component (Transform)
    entity->Assign<TransformComponent>();

	// Add it to the list
	m_entities.push_back(entity);
	return(m_entities.back());
}

void World::AddEntity(Entity* entity) {
    // Make sure our counter is larger
    m_entityCounter = std::max(m_entityCounter, entity->ID() + 1);
    
    std::vector<Entity*>::iterator it = m_entities.begin();
    for(; it != m_entities.end(); it++) {
        if((*it)->ID() == entity->ID()) {
            GIGA_ASSERT(false, "Trying to add an entity that already exists.");
            return;
        }
    }
    
    m_entities.push_back(entity);
}

Entity* World::FindEntity(int entityID) {
	std::vector<Entity*>::iterator it = m_entities.begin();
	for (; it != m_entities.end(); it++) {
		if ((*it)->m_entityID == entityID) {
			return((*it));
		}
	}

	return(0);
}

void World::Clear() {
	std::vector<Entity*>::iterator it = m_entities.begin();
	for (; it != m_entities.end(); it++) {
		delete (*it);
	}

	m_entities.clear();
}

System* World::FindSystem(std::string name) {
    std::vector<RegisteredSystem*>::iterator it = m_systems.begin();
    for(; it != m_systems.end(); it++) {
        if((*it)->system->GetGigaName() == name) {
            return((*it)->system);
        }
    }
    
    return(0);
}
