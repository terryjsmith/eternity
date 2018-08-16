
#ifndef dataobject_h
#define serialized_h

#include <eternity.h>
#include <Core/DataRecord.h>
#include <Core/GigaObject.h>

class GIGA_API DataObject : public GigaObject {
public:
    DataObject() = default;
    virtual ~DataObject() = default;
    
    /**
     * Serialize/deserialize from data record
     */
    virtual void Serialize(DataRecord* record) { }
    virtual void Deserialize(DataRecord* record) { }
    
    /**
     * Hooks after de/serializing
     */
    virtual void PreSerialize() { }
    virtual void PostSerialize() { }
    
    virtual void PreDeserialize() { }
    virtual void PostDeserialize() { }
};

#endif
