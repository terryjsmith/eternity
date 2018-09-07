
#include <Core/Component.h>
#include <Core/Entity.h>

std::map<std::string, Component::ComponentType*> Component::m_types;

Component::Component() {
    m_typeID = 0;
    m_parent = 0;
    m_updated = false;
    m_active = false;
}

uint32_t Component::GetTypeID() {
    if(m_typeID == 0) {
        std::map<std::string, ComponentType*>::iterator it = m_types.find(this->GetGigaName());
        GIGA_ASSERT(it != m_types.end(), "Component type not registered.");
        
        m_typeID = it->second->typeID;
    }
    
    return(m_typeID);
}

Component* Component::CreateComponent(std::string className) {
    std::map<std::string, ComponentType*>::iterator it = m_types.find(className);
    GIGA_ASSERT(it != m_types.end(), "Component type not registered.");

    return(it->second->ctor());
}

Component* Component::CreateComponent(uint32_t type) {
    std::map<std::string, ComponentType*>::iterator it = m_types.begin();
    for(; it != m_types.end(); it++) {
        if(it->second->typeID == type) {
            return(it->second->ctor());
        }
    }
    
    return(0);
}

std::vector<std::string> Component::GetComponentTypes() {
	std::vector<std::string> ret;
	std::map<std::string, ComponentType*>::iterator it = m_types.begin();
	for (; it != m_types.end(); it++) {
		ret.push_back(it->first);
	}

	return(ret);
}

bool Component::Active() {
    bool active = m_active;
    if(m_parent) {
        active &= m_parent->IsActive();
    }
    
    return(active);
}
