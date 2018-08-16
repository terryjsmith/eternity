
#ifndef sqlitedataloader_h
#define sqlitedataloader_h

#include <eternity.h>
#include <IO/DataLoader.h>

#include <sqlite3.h>

class GIGA_API SQLiteDataLoader : public DataLoader {
public:
    SQLiteDataLoader();
    ~SQLiteDataLoader();
    
    /**
     * Open a connection
     */
    void Open(std::string filename);
    
    /**
     * Close connection
     */
    void Close();
    
    /**
     * Query for records
     */
    std::vector<DataRecord*> GetRecords(std::string type, std::map<std::string, std::string> search = std::map<std::string, std::string>());
    
    /**
     * Save records
     */
    void SaveRecords(std::vector<DataRecord*> records);
    
protected:
    static int InternalDataCallback(void* instance, int count, char** data, char** cols);
    
    /**
     * Update table structures
     */
    void UpdateTables();
    
protected:
    // sqlite handle
    sqlite3* m_handle;
    
    // Current class/record being loaded
    std::string m_currentClassName;
    std::vector<DataRecord*> m_currentResults;
    DataRecord* m_returnedObject;
};

#endif
