
#include <IO/Serializer.h>
#include <IO/MemoryWriter.h>
#include <IO/MemoryReader.h>
#include <Core/MetaSystem.h>
#include <Core/Application.h>

unsigned char* Serializer::Serialize(DataRecord* record, uint32_t& size) {
    unsigned char* data = (unsigned char*)malloc(8000);
    
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(data, 8000);
    
    // Write object type
    DataRecordType* t = record->GetType();
    uint32_t type = t->GetTypeID();
    writer->Write(&type, sizeof(uint32_t));
    
    std::map<std::string, int> fields = t->GetKeys();
    std::map<std::string, int>::iterator f = fields.begin();
    for(; f != fields.end(); f++) {
        if(f->second == Variant::VAR_INT32 || f->second == Variant::VAR_UINT32) {
            int32_t i = record->Get(f->first)->AsInt();
            writer->Write(&i, sizeof(int32_t));
            continue;
        }
        
        if(f->second == Variant::VAR_INT64 || f->second == Variant::VAR_UINT64) {
            int64_t i = record->Get(f->first)->AsInt64();
            writer->Write(&i, sizeof(int64_t));
            continue;
        }
        
        if(f->second == Variant::VAR_BOOL) {
            bool b = record->Get(f->first)->AsBool();
            writer->Write(&b, sizeof(bool));
            continue;
        }
        
        if(f->second == Variant::VAR_FLOAT) {
            float fl = record->Get(f->first)->AsFloat();
            writer->Write(&fl, sizeof(float));
            continue;
        }
        
        if(f->second == Variant::VAR_VECTOR2) {
            vector2 v2 = record->Get(f->first)->AsVector2();
            writer->Write(&v2.x, sizeof(float));
            writer->Write(&v2.y, sizeof(float));
            continue;
        }
        
        if(f->second == Variant::VAR_VECTOR3) {
            vector3 v3 = record->Get(f->first)->AsVector3();
            writer->Write(&v3.x, sizeof(float));
            writer->Write(&v3.y, sizeof(float));
            writer->Write(&v3.z, sizeof(float));
            continue;
        }
        
        if(f->second == Variant::VAR_VECTOR4) {
            vector4 v4 = record->Get(f->first)->AsVector4();
            writer->Write(&v4.x, sizeof(float));
            writer->Write(&v4.y, sizeof(float));
            writer->Write(&v4.z, sizeof(float));
            writer->Write(&v4.w, sizeof(float));
            continue;
        }
        
        if(f->second == Variant::VAR_QUATERNION) {
            quaternion q = record->Get(f->first)->AsQuaternion();
            writer->Write(&q.w, sizeof(float));
            writer->Write(&q.x, sizeof(float));
            writer->Write(&q.y, sizeof(float));
            writer->Write(&q.z, sizeof(float));
            continue;
        }
        
        if(f->second == Variant::VAR_STRING) {
            std::string s = record->Get(f->first)->AsString();
            uint32_t nsize = s.size() + 1;
            writer->Write(&nsize, sizeof(uint32_t));
            
            char* str = (char*)malloc(nsize);
            memcpy(str, s.data(), s.size());
            str[nsize - 1] = '\0';
            writer->Write(str, nsize);
            free(str);
            continue;
        }
    }

    delete writer;
    
    size = writer->GetPosition();
    
    return(data);
}

void Serializer::Deserialize(unsigned char* data, uint32_t& size, DataRecord* record) {
    GIGA_ASSERT(record != 0, "Must provide pre-existing data record object.");
    
    // Create a memory reader
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(data, size);
    
    // Read data record type
    uint32_t typeID = 0;
    reader->Read(&typeID, sizeof(uint32_t));

    // And create an object
    DataRecordType* type = DataRecordType::GetType(typeID);
    
    std::map<std::string, int> keys = type->GetKeys();
    std::map<std::string, int>::iterator k = keys.begin();
    for(; k != keys.end(); k++) {
        if(k->second == Variant::VAR_INT32) {
            int32_t i = 0;
            reader->Read(&i, sizeof(int32_t));
            record->Set(k->first, new Variant(i));
            continue;
        }
        
        if(k->second == Variant::VAR_UINT32) {
            uint32_t i = 0;
            reader->Read(&i, sizeof(uint32_t));
            record->Set(k->first, new Variant(i));
            continue;
        }
        
        if(k->second == Variant::VAR_INT64) {
            int64_t i = 0;
            reader->Read(&i, sizeof(int64_t));
            record->Set(k->first, new Variant(i));
            continue;
        }
        
        if(k->second == Variant::VAR_UINT64) {
            uint64_t i = 0;
            reader->Read(&i, sizeof(uint64_t));
            record->Set(k->first, new Variant(i));
            continue;
        }
        
        if(k->second == Variant::VAR_BOOL) {
            bool b = false;
            reader->Read(&b, sizeof(bool));
            record->Set(k->first, new Variant(b));
            continue;
        }
        
        if(k->second == Variant::VAR_FLOAT) {
            float fl = 0;
            reader->Read(&fl, sizeof(float));
            record->Set(k->first, new Variant(fl));
            continue;
        }
        
        if(k->second == Variant::VAR_VECTOR2) {
            vector2 v;
            reader->Read(&v.x, sizeof(float));
            reader->Read(&v.y, sizeof(float));
            record->Set(k->first, new Variant(v));
            continue;
        }
        
        if(k->second == Variant::VAR_VECTOR3) {
            vector3 v;
            reader->Read(&v.x, sizeof(float));
            reader->Read(&v.y, sizeof(float));
            reader->Read(&v.z, sizeof(float));
            record->Set(k->first, new Variant(v));
            continue;
        }
        
        if(k->second == Variant::VAR_VECTOR4) {
            vector4 v;
            reader->Read(&v.x, sizeof(float));
            reader->Read(&v.y, sizeof(float));
            reader->Read(&v.z, sizeof(float));
            reader->Read(&v.w, sizeof(float));
            record->Set(k->first, new Variant(v));
            continue;
        }
        
        if(k->second == Variant::VAR_QUATERNION) {
            quaternion q;
            reader->Read(&q.w, sizeof(float));
            reader->Read(&q.x, sizeof(float));
            reader->Read(&q.y, sizeof(float));
            reader->Read(&q.z, sizeof(float));
            record->Set(k->first, new Variant(q));
            continue;
        }
        
        if(k->second == Variant::VAR_STRING) {
            uint32_t nsize = 0;
            reader->Read(&nsize, sizeof(uint32_t));
            
            std::string s;
            s.resize(nsize);
            
            reader->Read((void*)s.data(), nsize);
            
            record->Set(k->first, new Variant(s));
            continue;
        }
    }
    
    size = reader->GetPosition();
    delete reader;
}
