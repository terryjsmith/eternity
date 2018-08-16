
#ifndef dataobject_h
#define dataobject_h

#include <eternity.h>
#include <Core/GigaObject.h>

class DataRecord;

class GIGA_API DataObject : public GigaObject {
public:
    DataObject() = default;
    virtual ~DataObject() = default;
    
    /**
     * Serialize/deserialize from data record
     */
    virtual void Serialize(DataRecord* record) { }
    virtual void Deserialize(DataRecord* record) { }
};

#endif
