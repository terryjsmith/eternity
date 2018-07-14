
#include <Core/Entity.h>
#include <Core/World.h>
#include <Core/Application.h>

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

void Entity::AddComponent(Component* component, bool notify) {
    // Register with appropriate system
	World* world = Application::GetInstance()->GetWorld();
	std::vector<ComponentSystemBase*> systems = world->GetComponentSystems();
	std::vector<ComponentSystemBase*>::iterator it = systems.begin();

	for (; it != systems.end(); it++) {
		(*it)->AddComponent(component);
	}
    
    // Send message to other components
    if(notify) {
        std::vector<Component*>::iterator ci = m_components.begin();
        for(; ci != m_components.end(); ci++) {
            (*ci)->OnComponentAdded(component);
        }
    }
    
	component->m_parent = this;
    component->OnEntityAssigned();
    m_components.push_back(component);
}

Component* Entity::Assign(std::string className) {
    // Register with appropriate system
    World* world = Application::GetInstance()->GetWorld();
    std::vector<ComponentSystemBase*> systems = world->GetComponentSystems();
    std::vector<ComponentSystemBase*>::iterator it = systems.begin();
    
    Component* component = Component::CreateComponent(className);
    for(; it != systems.end(); it++) {
        (*it)->AddComponent(component);
    }
    
    // Send message to other components
    std::vector<Component*>::iterator ci = m_components.begin();
    for(; ci != m_components.end(); ci++) {
        (*ci)->OnComponentAdded(component);
    }
    
    component->m_parent = this;
    component->OnEntityAssigned();
    m_components.push_back(component);
    
    // Return
    return(component);
}

void Entity::PostDeserialize() {
	
}
