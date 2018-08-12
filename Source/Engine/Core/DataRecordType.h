
#ifndef datarecordtype_h
#define datarecordtype_h

#include <eternity.h>
#include <Core/GigaObject.h>

class GIGA_API DataRecordType {
public:
    DataRecordType() = default;
    DataRecordType(std::string typeName) : m_typeName(typeName) { }
    ~DataRecordType();

	struct DataRecordField {
		std::string name;
		int type;
		bool editable;
		std::string friendly_name;
	};
    
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
    void AddKey(std::string name, int type, bool editable = true);
    int GetKeyType(std::string name);
	DataRecordField* GetKeyDetail(std::string name);

	/**
	 * Set key properties
	 */
	void SetKeyNonEditable(std::string key);
	void SetKeyFriendlyName(std::string key, std::string name);

    /**
     * Get all keys
     */
    std::map<std::string, int> GetKeys() { return m_keys; }
    
    /**
     * Get type ID (int)
     */
    uint32_t GetTypeID() { return m_typeID; }
    
    /**
     * Register a new type
     */
    template<class T>
    static void Register(std::string name, DataRecordType* type) {
        int newTypeID = ++m_lastTypeID;
        m_types[name] = type;
        m_types[name]->m_typeID = newTypeID;
        m_ctors[newTypeID] = &InternalCreateObject<T>;
    }
    
    /**
     * Find a type
     */
    static DataRecordType* GetType(std::string name);
    static DataRecordType* GetType(uint32_t typeID);
    
    /**
     * Get all registered types
     */
    static std::map<std::string, DataRecordType*>& GetAllTypes() { return m_types; }
    
    /**
     * Create a new object from a DataRecordType ID
     */
    static GigaObject* CreateObject(uint32_t dataRecordTypeID);
    
protected:
    // Create a new component from a class type
    template<typename T> static GigaObject* InternalCreateObject() { return new T; }
    
protected:
    // Our type name
    std::string m_typeName;
    
    // Type ID
    uint32_t m_typeID;
    
    // Primary key/lookup column
    std::string m_primaryKey;
    
    // Listing of keys and types (from Variant::Type)
	std::map<std::string, int> m_keys;

	// Key details
	std::map<std::string, DataRecordField*> m_details;
    
    // Registered type creator
    static std::map<int, GigaObject*(*)()> m_ctors;
    
    // All registered types
    static std::map<std::string, DataRecordType*> m_types;
    
    // Last used type ID
    static uint32_t m_lastTypeID;
};

#endif
