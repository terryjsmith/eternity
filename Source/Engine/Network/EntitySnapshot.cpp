
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
    std::map<int, DataRecord*>::iterator ei = entityRecords.begin();
    for(; ei != entityRecords.end(); ei++) {
        delete ei->second;
    }
    entityRecords.clear();
    
    std::map<int, std::map<int, DataRecord*>>::iterator ci = componentRecords.begin();
    for(; ci != componentRecords.end(); ci++) {
        std::map<int, DataRecord*>::iterator di = ci->second.begin();
        for(; di != ci->second.end(); di++) {
            delete di->second;
        }
    }
    
    componentRecords.clear();
    
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
    uint32_t count = entityRecords.size();
    writer->Write(&count, sizeof(uint32_t));
    
    // Iterate over entities first
    std::map<int, DataRecord*>::iterator ei = entityRecords.begin();
    for(; ei != entityRecords.end(); ei++) {
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
    std::map<int, std::map<int, DataRecord*>>::iterator mi = componentRecords.begin();
    for(; mi != componentRecords.end(); mi++) {
        count += mi->second.size();
    }
    
    // Write number of components
    writer->Write(&count, sizeof(uint32_t));
    
    // Then write components
    mi = componentRecords.begin();
    for(; mi != componentRecords.end(); mi++) {
        std::map<int, DataRecord*>::iterator di = mi->second.begin();
        for(; di != mi->second.end(); di++) {
            // Write entity ID
            writer->Write((int*)&mi->first, sizeof(uint32_t));
            
            // Write component type
            uint32_t componentType = di->first;
            writer->Write(&componentType, sizeof(uint32_t));
            
            // Write record type
            DataRecordType* t = di->second->GetType();
            uint32_t recordType = t->GetTypeID();
            writer->Write(&recordType, sizeof(uint32_t));
                
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
        DataRecord* entityRecord = new DataRecord("Entity");
        
        uint32_t nsize = m_bufferSize - reader->GetPosition();
        serializer->Deserialize(reader->GetCurrent(), nsize, entityRecord);
        entityRecords[entityID] = entityRecord;
        componentRecords[entityID] = std::map<int, DataRecord*>();
        
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
        
        // Data record type
        uint32_t recordType = 0;
        reader->Read(&recordType, sizeof(uint32_t));
        
        // Read component
        DataRecord* componentRecord = new DataRecord(recordType);
        
        uint32_t nsize = m_bufferSize - reader->GetPosition();
        serializer->Deserialize(reader->GetCurrent(), nsize, componentRecord);
        
        componentRecords[entityID][componentType] = componentRecord;
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
    std::map<int, DataRecord*>::iterator it = entityRecords.find(entityID);
    if(it != entityRecords.end()) {
        delete it->second;
        entityRecords.erase(it);
    }
    
    DataRecord* entityRecord = new DataRecord("Entity");
    entity->Serialize(entityRecord);
    
    entityRecords[entityID] = entityRecord;
}

void EntitySnapshot::AddComponent(Component* component) {
    int entityID = component->GetParent()->ID();
    std::map<int, std::map<int, DataRecord*>>::iterator it = componentRecords.find(entityID);
    if(it == componentRecords.end()) {
        this->AddEntity(component->GetParent());
        componentRecords[entityID] = std::map<int, DataRecord*>();
        it = componentRecords.find(entityID);
    }
    
    int componentType = component->GetTypeID();
    std::map<int, DataRecord*>::iterator ci = it->second.find(componentType);
    if(ci != it->second.end()) {
        delete ci->second;
        it->second.erase(ci);
    }
    
    DataRecord* componentRecord = new DataRecord(component->GetGigaName());
    component->Serialize(componentRecord);
    
    componentRecords[entityID][componentType] = componentRecord;
}

void EntitySnapshot::Clone(EntitySnapshot* other) {
    // Copy entity records
    other->entityRecords = entityRecords;
    other->componentRecords = componentRecords;
    
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
