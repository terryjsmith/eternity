
#include <Core/Component.h>

std::map<std::string, Component::ComponentType*> Component::m_types;

Component::Component() {
    m_typeID = 0;
    m_parent = 0;
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
