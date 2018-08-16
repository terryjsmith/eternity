
#ifndef dataloader_h
#define dataloader_h

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/DataRecord.h>
#include <Core/Service.h>

class GIGA_API DataLoader : public Service {
public:
    ~DataLoader() = default;
    
    /**
     * Open a connection
     */
    virtual void Open(std::string location) { }
    
    /**
     * Close connection
     */
    virtual void Close() { }
    
    /**
     * Query for records
     */
    virtual std::vector<DataRecord*> GetRecords(std::string type, std::map<std::string, std::string> search = std::map<std::string, std::string>()) = 0;
    
    /**
     * Save records
     */
    virtual void SaveRecords(std::vector<DataRecord*> records) = 0;
    
protected:
    // No direct creation
    DataLoader() = default;
    
protected:
    // Open connection
    std::string m_location;
};

#endif
