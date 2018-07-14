
#ifndef dataloader_h
#define dataloader_h

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/DataRecord.h>
#include <Core/Service.h>

class GIGA_API DataLoader : public Service {
public:
    ~DataLoader();
    
    /**
     * Open a connection
     */
    virtual void Open(std::string location) { }
    
    /**
     * Close connection
     */
    virtual void Close() { }
    
    /**
     * Add a new record
     */
    void AddRecord(GigaObject* record);
    
    /**
     * Query for records
     */
    virtual std::vector<DataRecord*> GetRecords(std::string type) = 0;
    
    /**
     * Save records
     */
    virtual void SaveRecords() = 0;
    
protected:
    // No direct creation
    DataLoader() = default;
    
protected:
    // Open connection
    std::string m_location;
    
    // Stored type->records map
    std::map<std::string, std::vector<DataRecord*>> m_records;
};

#endif
