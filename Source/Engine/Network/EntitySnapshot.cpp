
#include <Network/EntitySnapshot.h>
#include <IO/MemoryWriter.h>
#include <IO/MemoryReader.h>
#include <Core/DataRecord.h>
#include <Network/NetworkSystem.h>
#include <IO/Serializer.h>

EntitySnapshot::EntitySnapshot() {
    m_buffer = 0;
    m_bufferSize = 0;
}

EntitySnapshot::~EntitySnapshot() {
    std::map<int, DataRecord*>::iterator ei = m_entityRecords.begin();
    for(; ei != m_entityRecords.end(); ei++) {
        delete ei->second;
    }
    m_entityRecords.clear();
    
    std::map<int, std::map<int, DataRecord*>>::iterator ci = m_componentRecords.begin();
    for(; ci != m_componentRecords.end(); ci++) {
        std::map<int, DataRecord*>::iterator di = ci->second.begin();
        for(; di != ci->second.end(); di++) {
            delete di->second;
        }
    }
    
    m_componentRecords.clear();
    
    if(m_buffer) {
        free(m_buffer);
        m_bufferSize = 0;
    }
}

void EntitySnapshot::Serialize() {
    if(m_buffer) {
        free(m_buffer);
        m_bufferSize = 0;
    }
    
    m_buffer = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);
    m_bufferSize = NETWORK_MAX_PACKET_SIZE;
    memset(m_buffer, 0, NETWORK_MAX_PACKET_SIZE);
    
    // Keep track of our current buffersize in a memory writer
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(m_buffer, m_bufferSize);
    writer->Resizable(true, NETWORK_MAX_PACKET_SIZE);
    
    // Write type
    uint32_t t = type;
    writer->Write(&t, sizeof(uint32_t));
    
    // Initialize a serializer
    Serializer* serializer = new Serializer();
    
    // Write number of entities
    uint32_t count = m_entityRecords.size();
    writer->Write(&count, sizeof(uint32_t));
    
    // Iterate over entities first
    std::map<int, DataRecord*>::iterator ei = m_entityRecords.begin();
    for(; ei != m_entityRecords.end(); ei++) {
        // Write entity ID
        uint32_t entityID = ei->first;
        writer->Write(&entityID, sizeof(uint32_t));
        
        // Write entity
        uint32_t size = 0;
        unsigned char* data = serializer->Serialize(ei->second, size);
        
        writer->Write(data, size);
        free(data);
    }
    
    // Get component count
    count = 0;
    std::map<int, std::map<int, DataRecord*>>::iterator mi = m_componentRecords.begin();
    for(; mi != m_componentRecords.end(); mi++) {
        count += mi->second.size();
    }
    
    // Write number of components
    writer->Write(&count, sizeof(uint32_t));
    
    // Then write components
    mi = m_componentRecords.begin();
    for(; mi != m_componentRecords.end(); mi++) {
        std::map<int, DataRecord*>::iterator di = mi->second.begin();
        for(; di != mi->second.end(); di++) {
            // Write entity ID
            writer->Write((int*)&mi->first, sizeof(uint32_t));
            
            // Write component type
            uint32_t componentType = di->first;
            writer->Write(&componentType, sizeof(uint32_t));
                
            // Write component
            uint32_t size = 0;
            unsigned char* data = serializer->Serialize(di->second, size);
                
            writer->Write(data, size);
            free(data);
        }
    }
    
    m_bufferSize = writer->GetPosition();
    delete writer;
}

void EntitySnapshot::Deserialize() {
    // Deserialize buffer, start with reading data out
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(m_buffer, m_bufferSize);
    
    // Read type
    uint32_t t = type;
    reader->Read(&t, sizeof(uint32_t));
    type = t;
    
    // Read number of entities
    uint32_t count = 0;
    reader->Read(&count, sizeof(uint32_t));
    
    // Initialize a deserializer
    Serializer* serializer = new Serializer();
    
    // Read entities
    for(uint32_t i = 0; i < count; i++) {
        // Read entity ID
        uint32_t entityID = 0;
        reader->Read(&entityID, sizeof(uint32_t));
        
        // Read entity
        DataRecord* entityRecord = new DataRecord();
        
        uint32_t nsize = m_bufferSize - reader->GetPosition();
        serializer->Deserialize(reader->GetCurrent(), nsize, entityRecord);
        m_entityRecords[entityID] = entityRecord;
        m_componentRecords[entityID] = std::map<int, DataRecord*>();
        
        reader->SetPosition(reader->GetPosition() + nsize);
    }
    
    // Then read component count
    reader->Read(&count, sizeof(uint32_t));
    
    // Then read components
    for(uint32_t i = 0; i < count; i++) {
        // Read entity ID
        uint32_t entityID = 0;
        reader->Read(&entityID, sizeof(uint32_t));
        
        // Read component type
        uint32_t componentType = 0;
        reader->Read(&componentType, sizeof(uint32_t));
        
        // Read component
        DataRecord* componentRecord = new DataRecord();
        
        uint32_t nsize = m_bufferSize - reader->GetPosition();
        serializer->Deserialize(reader->GetCurrent(), nsize, componentRecord);
        
        m_componentRecords[entityID][componentType] = componentRecord;
        reader->SetPosition(reader->GetPosition() + nsize);
    }
    
    delete reader;
}

unsigned char* EntitySnapshot::GetData(int& size) {
    size = m_bufferSize;
    return(m_buffer);
}

void EntitySnapshot::AddEntity(Entity* entity) {
    int entityID = entity->ID();
    std::map<int, DataRecord*>::iterator it = m_entityRecords.find(entityID);
    if(it != m_entityRecords.end()) {
        delete it->second;
        m_entityRecords.erase(it);
    }
    
    DataRecord* entityRecord = new DataRecord();
    entity->Serialize(entityRecord);
    entityRecord->SetObject(entity);
    
    m_entityRecords[entityID] = entityRecord;
}

void EntitySnapshot::AddComponent(Component* component) {
    int entityID = component->GetParent()->ID();
    std::map<int, std::map<int, DataRecord*>>::iterator it = m_componentRecords.find(entityID);
    if(it == m_componentRecords.end()) {
        this->AddEntity(component->GetParent());
        m_componentRecords[entityID] = std::map<int, DataRecord*>();
        it = m_componentRecords.find(entityID);
    }
    
    int componentType = component->GetTypeID();
    std::map<int, DataRecord*>::iterator ci = it->second.find(componentType);
    if(ci != it->second.end()) {
        delete ci->second;
        it->second.erase(ci);
    }
    
    DataRecord* componentRecord = new DataRecord();
    component->Serialize(componentRecord);
    componentRecord->SetObject(component);
    
    m_componentRecords[entityID][componentType] = componentRecord;
}

std::vector<Entity*> EntitySnapshot::GetEntities() {
    std::map<int, Entity*> entities;
    
    // Deserialize entities
    std::map<int, DataRecord*>::iterator it = m_entityRecords.begin();
    for(; it != m_entityRecords.end(); it++) {
        Entity* entity = new Entity();
        entity->Deserialize(it->second);
        entity->PostDeserialize();
        
        entities[entity->ID()] = entity;
    }
    
    // Then components
    std::map<int, std::map<int, DataRecord*>>::iterator ci = m_componentRecords.begin();
    for(; ci != m_componentRecords.end(); ci++) {
        std::map<int, DataRecord*>::iterator mi = ci->second.begin();
        for(; mi != ci->second.end(); mi++) {
            // Create components
            Component* component = Component::CreateComponent(mi->first);
            component->SetParent(entities[ci->first]);
            component->Deserialize(mi->second);
            component->PostDeserialize();
            
            entities[ci->first]->AddComponent(component);
        }
    }
    
    std::vector<Entity*> entityList;
    std::map<int, Entity*>::iterator ei = entities.begin();
    for(; ei != entities.end(); ei++) {
        entityList.push_back(ei->second);
    }
    
    return(entityList);
}

std::vector<Entity*> EntitySnapshot::Interpolate(EntitySnapshot* first, EntitySnapshot* second, float interpolate) {
    std::map<int, Entity*> entities;
    
    // Deserialize entities from first snapshot, use those as base
    std::map<int, DataRecord*>::iterator it = first->m_entityRecords.begin();
    for(; it != first->m_entityRecords.end(); it++) {
        Entity* entity = new Entity();
        entity->Deserialize(it->second);
        entity->PostDeserialize();
        
        entities[entity->ID()] = entity;
    }
    
    // Iterate over components from first snapshot, baseline
    std::map<int, std::map<int, DataRecord*>>::iterator ci = first->m_componentRecords.begin();
    
    // Check for the existence of the entity in the next snapshot
    std::map<int, std::map<int, DataRecord*>>::iterator si = second->m_componentRecords.find(ci->first);
    
    for(; ci != first->m_componentRecords.end(); ci++) {
        std::map<int, DataRecord*>::iterator mi = ci->second.begin();
        
        for(; mi != ci->second.end(); mi++) {
            Component* component = Component::CreateComponent(mi->first);
            component->SetParent(entities[ci->first]);
            
            // Check for existence of component in next snapshot
            std::map<int, DataRecord*>::iterator smi = si->second.find(mi->first);
                                                                       
            // First value
            DataRecord* dr1 = mi->second;
            
            // If no second value, just use the first value
            if(smi == si->second.end()) {
                component->Deserialize(dr1);
                component->PostDeserialize();
                continue;
            }
            
            // Create a new data record
            DataRecord* drc = new DataRecord();
            
            // Otherwise, need to interpolate over values
            DataRecordType* drt = dr1->GetType();
            std::map<std::string, int> fields = drt->GetKeys();
            std::map<std::string, int>::iterator fi = fields.begin();
            for(; fi != fields.end(); fi++) {
                uint32_t fieldType = fi->second;
                if(fieldType == Variant::VAR_INT32 || fieldType == Variant::VAR_INT64 || fieldType == Variant::VAR_UINT32 ||
                   fieldType == Variant::VAR_UINT64) {
                    int i1 = mi->second->Get(fi->first)->AsInt();
                    int i2 = smi->second->Get(fi->first)->AsInt();
                    
                    drc->Set(fi->first, new Variant(i1 + ((i2  - i1) * interpolate)));
                    continue;
                }
                
                if(fieldType == Variant::VAR_FLOAT) {
                    float i1 = mi->second->Get(fi->first)->AsFloat();
                    float i2 = smi->second->Get(fi->first)->AsFloat();
                    
                    drc->Set(fi->first, new Variant(i1 + ((i2 - i1) * interpolate)));
                    continue;
                }
                
                if(fieldType == Variant::VAR_VECTOR2) {
                    vector2 v1 = mi->second->Get(fi->first)->AsVector2();
                    vector2 v2 = smi->second->Get(fi->first)->AsVector2();
                    
                    drc->Set(fi->first, new Variant(v1 + ((v2 - v1)  * interpolate)));
                    continue;
                }
                
                if(fieldType == Variant::VAR_VECTOR3) {
                    vector3 v1 = mi->second->Get(fi->first)->AsVector3();
                    vector3 v2 = smi->second->Get(fi->first)->AsVector3();
                    
                    drc->Set(fi->first, new Variant(v1 + ((v2 - v1)  * interpolate)));
                    continue;
                }

                if(fieldType == Variant::VAR_VECTOR4) {
                    vector4 v1 = mi->second->Get(fi->first)->AsVector4();
                    vector4 v2 = smi->second->Get(fi->first)->AsVector4();
                    
                    drc->Set(fi->first, new Variant(v1 + ((v2 - v1)  * interpolate)));
                    continue;
                }
                
                if(fieldType == Variant::VAR_QUATERNION) {
                    quaternion q1 = mi->second->Get(fi->first)->AsQuaternion();
                    quaternion q2 = smi->second->Get(fi->first)->AsQuaternion();
                    
                    quaternion result = glm::lerp(q1, q2, interpolate);
                    drc->Set(fi->first, new Variant(result));
                    continue;
                }
                
                // For strings, bool, etc. just set to first value
                drc->Set(fi->first, mi->second->Get(fi->first));
            }
            
            component->Deserialize(drc);
            component->PostDeserialize();
            delete drc;
            
            entities[ci->first]->AddComponent(component);
        }
    }
    
    std::vector<Entity*> entityList;
    std::map<int, Entity*>::iterator ei = entities.begin();
    for(; ei != entities.end(); ei++) {
        entityList.push_back(ei->second);
    }
    
    return(entityList);
}

void EntitySnapshot::Clone(EntitySnapshot* other) {
    // Copy entity records
    other->m_entityRecords = m_entityRecords;
    other->m_componentRecords = m_componentRecords;
    
    if(m_bufferSize) {
        other->m_bufferSize = m_bufferSize;
        other->m_buffer = (unsigned char*)malloc(m_bufferSize);
        memcpy(other->m_buffer, m_buffer, m_bufferSize);
    }
}

void EntitySnapshot::SetData(unsigned char* buffer, int size) {
    m_buffer = (unsigned char*)malloc(size);
    memcpy(m_buffer, buffer, size);
    m_bufferSize = size;
}
