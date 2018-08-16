
#include <Core/Entity.h>
#include <Core/World.h>
#include <Core/Application.h>
#include <Core/DataRecord.h>

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

	Entity* e = world->FindEntity(id);
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

Component* Entity::FindComponent(uint32_t typeID) {
    std::vector<Component*>::iterator it = m_components.begin();
    for(; it != m_components.end(); it++) {
        if((*it)->GetTypeID() == typeID) {
            return(*it);
        }
    }
    
    return(0);
}

void Entity::AddComponent(Component* component, bool notify) {
	std::vector<Component*>::iterator ci = std::find(m_components.begin(), m_components.end(), component);
	if (ci != m_components.end()) {
		return;
	}

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
    
	component->SetParent(this);
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
    
    component->SetParent(this);
    component->OnEntityAssigned();
    m_components.push_back(component);
    
    // Return
    return(component);
}

void Entity::Deserialize(DataRecord* record) {
    m_entityID = record->Get("id")->AsInt();
    m_entityName = record->Get("name")->AsString();
    m_active = record->Get("active")->AsBool();
}

void Entity::Serialize(DataRecord* record) {
    record->Set("id", new Variant(m_entityID));
    record->Set("name", new Variant(m_entityName));
    record->Set("active", new Variant(m_active));
}
