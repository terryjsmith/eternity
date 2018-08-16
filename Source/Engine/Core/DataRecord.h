
#ifndef datarecord_h
#define datarecord_h

#include <eternity.h>
#include <Core/Variant.h>
#include <Core/DataObject.h>
#include <Core/DataRecordType.h>

/**
 * A data record represents a single row or data record loaded or serialized as key/value pairs
 */
class GIGA_API DataRecord {
public:
    DataRecord(std::string type);
    DataRecord(uint32_t type);
    ~DataRecord();
    
    /**
     * Get/set record ID
     */
    int GetRecordID() { return m_recordID; }
    void SetRecordID(int recordID) { m_recordID = recordID; }
    
    /**
     * Get/set individual key/val pairs
     */
    Variant* Get(std::string key);
    std::string GetString(std::string key);
    void Set(std::string key, Variant* value);
    
    /**
     * Set key/val pair from string value
     */
    void Set(std::string key, std::string value);
    
    /**
     * Get all key/val pairs
     */
    std::map<std::string, Variant*> GetValues() { return m_values; }
    
    /**
     * Get data record type
     */
    DataRecordType* GetType();
    
    /**
     * Set/get deserialized status
     */
    bool IsDeserialized() { return m_deserialized; }
    void SetDeserialized(bool deserialized) { m_deserialized = deserialized; }
    
    /**
     * Set/get deleted status
     */
    void SetDeleted(bool deleted) { m_deleted = deleted; }
    bool IsDeleted() { return m_deleted; }
    
protected:
    // Saved record ID
    int m_recordID;
    
    // Key/value pairs
    std::map<std::string, Variant*> m_values;
    
    // Key/value strings
    std::map<std::string, std::string> m_strValues;
    
    // Data record type
    DataRecordType* m_type;
    
    // Deserialized into object?
    bool m_deserialized;
    
    // Is deleted
    bool m_deleted;
};

#endif
