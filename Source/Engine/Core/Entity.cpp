
#include <Core/Entity.h>

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

Component* Entity::FindComponent(std::string name) {
    std::vector<Component*>::iterator it = m_components.begin();
    for(; it != m_components.end(); it++) {
        if((*it)->GetGigaName() == name) {
            return(*it);
        }
    }
    
    return(0);
}
