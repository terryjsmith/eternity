
#ifndef serializer_hpp
#define serializer_hpp

#include <eternity.h>
#include <Core/DataRecord.h>

class GIGA_API Serializer {
public:
    Serializer() = default;
    ~Serializer() = default;
    
    unsigned char* Serialize(DataRecord* record, uint32_t& size);
    void Deserialize(unsigned char* data, uint32_t& size, DataRecord* record);
};

#endif
