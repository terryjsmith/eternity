
#ifndef datarecordtype_h
#define datarecordtype_h

#include <eternity.h>

class GIGA_API DataRecordType {
public:
    DataRecordType() = default;
    DataRecordType(std::string typeName) : m_typeName(typeName) { }
    ~DataRecordType() = default;
    
    /**
     * Get/set type name
     */
    void SetTypeName(std::string typeName) { m_typeName = typeName; }
    std::string GetTypeName() { return m_typeName; }
    
    /**
     * Get/set primary key
     */
    void SetPrimaryKey(std::string key) { m_primaryKey = key; }
    std::string GetPrimaryKey() { return m_primaryKey; }
    
    /**
     * Add/get keys/types
     */
    void AddKey(std::string name, int type);
    int GetKeyType(std::string name);
    
    /**
     * Get all keys
     */
    std::map<std::string, int> GetKeys() { return m_keys; }
    
    /**
     * Register a new type
     */
    static void Register(std::string name, DataRecordType* type);
    
    /**
     * Find a type
     */
    static DataRecordType* GetType(std::string name);
    
    /**
     * Get all registered types
     */
    static std::map<std::string, DataRecordType*>& GetAllTypes() { return m_types; }
    
protected:
    // Our type name
    std::string m_typeName;
    
    // Primary key/lookup column
    std::string m_primaryKey;
    
    // Listing of keys and types (from Variant::Type)
    std::map<std::string, int> m_keys;
    
    // All registered types
    static std::map<std::string, DataRecordType*> m_types;
};

#endif
