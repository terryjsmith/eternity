
#include <IO/SQLiteDataLoader.h>
#include <Core/Error.h>
#include <Core/MetaSystem.h>
#include <Core/Application.h>

SQLiteDataLoader::SQLiteDataLoader() {
    m_handle = 0;
}

SQLiteDataLoader::~SQLiteDataLoader() {
    if(m_handle) {
        sqlite3_close(m_handle);
    }
}

void SQLiteDataLoader::Open(std::string filename) {
    int result = sqlite3_open(filename.c_str(), &m_handle);
    if(result != 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to ope SQLite file.", filename));
        sqlite3_close(m_handle);
        m_handle = 0;
    }
    
    UpdateTables();
}

void SQLiteDataLoader::Close() {
    if(m_handle) {
        sqlite3_close(m_handle);
        m_handle = 0;
    }
}

std::vector<DataRecord*> SQLiteDataLoader::GetRecords(std::string type, std::map<std::string, std::string> search) {
    m_currentClassName = type;
    std::vector<DataRecord*> ret;
    
    // Get type
    DataRecordType* drt = DataRecordType::GetType(type);
    GIGA_ASSERT(drt != 0, "Data record type not found.");
    
    // Construct our query
    std::map<std::string, int> fields = drt->GetKeys();
    GIGA_ASSERT(fields.size() > 0, "No fields defined in data record type.");
    
    std::map<std::string, int>::iterator field = fields.begin();
    std::string fieldList = drt->GetPrimaryKey();
    for(; field != fields.end(); field++) {
        fieldList += "," + field->first;
    }
    
    std::string query = "SELECT " + fieldList + " FROM " + type;

	// Add where clauses
	if (search.size()) {
		query += " WHERE 1=1 ";
		std::map<std::string, std::string>::iterator it = search.begin();
		for (; it != search.end(); it++) {
			query += " AND " + it->first + " = '" + it->second + "'";
		}
	}
    
    if(sqlite3_exec(m_handle, query.c_str(), InternalDataCallback, this, 0) != 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to get record list from SQLite.", (char*)sqlite3_errmsg(m_handle)));
        GIGA_ASSERT(false, "Unable to get record list from SQLite.");
        return(ret);
    }
    
    // Post-processing for any object types
    int count = m_currentResults.size();
    for(size_t i = 0; i < count; i++) {
        DataRecord* di = m_currentResults[i];
        ProcessRecord(this, di);
    }
    
    ret = std::vector<DataRecord*>(m_currentResults.begin(), m_currentResults.begin() + count);
    
    m_currentClassName = std::string();
    m_returnedObject = 0;
    m_currentResults.clear();
    return(ret);
}

int SQLiteDataLoader::InternalDataCallback(void* instance, int count, char** data, char** cols) {
    SQLiteDataLoader* loader = (SQLiteDataLoader*)instance;
    std::string className = loader->m_currentClassName;
    
    // Create a new object from our meta system
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    GigaObject* object = metaSystem->CreateObject(className);
    
    // Create a new data record for tracking
    DataRecord* record = new DataRecord();
    record->SetObject(object);
    
    // Get the record type
    DataRecordType* type = record->GetType();
    
    // Track the primary key
    unsigned int primaryKeyID = 0;
    
    // Iterate over count columns of data
    for(int i = 0; i < count; i++) {
        if(data[i] == 0) {
            continue;
        }
    
        record->Set(cols[i], data[i]);
        
        // If this is the primary key, convert and save it
        if(type->GetPrimaryKey().compare(cols[i]) == 0) {
            primaryKeyID = atoi(data[i]);
        }
    }
    
    // Make sure we don't already have this record in the list
    bool recordExists = false;
    std::vector<DataRecord*>::iterator di = loader->m_records[className].begin();
    for(; di != loader->m_records[className].end(); di++) {
        if((*di)->GetRecordID() == primaryKeyID) {
            recordExists = true;
            break;
        }
    }
    
    if(recordExists) {
        delete record;
        delete object;
        
        record = *di;
        object = record->GetObject();
    }
    else {
        record->SetRecordID(primaryKeyID);
    
        loader->m_records[className].push_back(record);
    }
    
    loader->m_currentResults.push_back(record);
    loader->m_returnedObject = record;
    
    return(0);
}

void SQLiteDataLoader::ProcessRecord(SQLiteDataLoader* loader, DataRecord* record) {
    // Get the record type
    DataRecordType* type = record->GetType();
    
    // Iterate over columns
    std::map<std::string, int> fields = type->GetKeys();
    std::map<std::string, int>::iterator f = fields.begin();
    for(; f != fields.end(); f++) {
        // Get column type
        int colType = type->GetKeyType(f->first);
        if(colType == Variant::VAR_OBJECT) {
            // Expand object, currently stored as a string with format TableName:ID
            std::string recordStr = record->GetString(f->first);
            std::string className = recordStr.substr(0, recordStr.find(":"));
            std::string recordID = recordStr.substr(recordStr.find(":") + 1);
            
            DataRecordType* drt = DataRecordType::GetType(className);
            
            // Construct our query
            std::map<std::string, int> fields = drt->GetKeys();
            GIGA_ASSERT(fields.size() > 0, "No fields defined in data record type.");
            
            std::map<std::string, int>::iterator field = fields.begin();
            std::string fieldList = drt->GetPrimaryKey();
            for(; field != fields.end(); field++) {
                fieldList += "," + field->first;
            }
            
            std::string query = "SELECT " + fieldList + " FROM " + className + " WHERE " + drt->GetPrimaryKey() + " = " + recordID;
            
            loader->m_currentClassName = className;
            if(sqlite3_exec(loader->m_handle, query.c_str(), InternalDataCallback, loader, 0) != 0) {
                Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to get record list from SQLite.", (char*)sqlite3_errmsg(loader->m_handle)));
                GIGA_ASSERT(false, "Unable to get record list from SQLite.");
            }
            
            record->Set(f->first, new Variant(loader->m_returnedObject->GetObject()));
            
            ProcessRecord(loader, loader->m_returnedObject);
        }
    }
    
    if(record->IsDeserialized() == false) {
        record->GetObject()->PreDeserialize();
        record->GetObject()->Deserialize(record);
        record->GetObject()->PostDeserialize();
        record->SetDeserialized(true);
    }
}

void SQLiteDataLoader::UpdateTables() {
    // Create tables where they do not exist
    std::map<std::string, DataRecordType*> types = DataRecordType::GetAllTypes();
    std::map<std::string, DataRecordType*>::iterator r = types.begin();
    for(; r != types.end(); r++) {
        // Get our primary key
        std::string primaryKey = r->second->GetPrimaryKey();
        
        std::string query = "CREATE TABLE IF NOT EXISTS ";
        query += r->first + " (";
        
        query += primaryKey + " INTEGER";
        
        std::map<std::string, int> fields = r->second->GetKeys();
        std::map<std::string, int>::iterator f = fields.begin();
        for(; f != fields.end(); f++) {
            query += "," + f->first + " TEXT";
        }
        
        query += ", PRIMARY KEY(" + primaryKey + " ASC))";
        
        if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
            Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to create SQLite table.", (char*)sqlite3_errmsg(m_handle)));
        }
    }
    
    // Alter table as necessary where columns may not exist
    r = types.begin();
    for(; r != types.end(); r++) {
        std::string primaryKey = r->second->GetPrimaryKey();

        // Get our column names
        std::string query = "PRAGMA table_info(" + r->first + ")";
        if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
            Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to get SQLite schema.", (char*)sqlite3_errmsg(m_handle)));
            return;
        }
        
        // Put the column names into a vector
        sqlite3_stmt* stmt = 0;
        int result = 0;
        std::vector<std::string> existingCols;
        result = sqlite3_prepare(m_handle, query.c_str(), (int)query.length() + 1, &stmt, 0);
        while((result = sqlite3_step(stmt))) {
            if(result != SQLITE_ROW) {
                break;
            }
            
            std::string colname = (const char*)sqlite3_column_text(stmt, 1);
            existingCols.push_back(colname);
        }
        
        std::map<std::string, int> fields = r->second->GetKeys();
        std::map<std::string, int>::iterator f = fields.begin();
        for(; f != fields.end(); f++) {
            if(f->first == primaryKey) continue;
            
            std::vector<std::string>::iterator it = std::find(existingCols.begin(), existingCols.end(), f->first);
            if(it == existingCols.end()) {
                query = "ALTER TABLE " + r->first + " ADD COLUMN " + f->first;
                
                if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                    Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to alter SQLite table.",     (char*)sqlite3_errmsg(m_handle)));
                    return;
                }
            }
        }
    }
}

void SQLiteDataLoader::CreateRecords(DataRecord* record) {
    // Get record type
    DataRecordType* type = record->GetType();
    std::string query = "";
    
    // Update object references
    std::map<std::string, int> fields = type->GetKeys();
    std::map<std::string, int>::iterator f = fields.begin();
    for(; f != fields.end(); f++) {
        if(type->GetKeyType(f->first) == Variant::VAR_OBJECT) {
            // Only want to update objects with no reference
            std::string lookup = record->GetString(f->first);
            if(lookup.empty()) {
                // Try to find another matching data record first
                GigaObject* obj = record->Get(f->first)->AsObject();
                std::string className = obj->GetGigaName();
                std::map<std::string, std::vector<DataRecord*>>::iterator ci = m_records.find(className);
                
                bool found = false;
                if(ci != m_records.end()) {
                    std::vector<DataRecord*>::iterator it = ci->second.begin();
                    for(; it != ci->second.end(); it++) {
                        if((*it)->GetObject() == obj) {
                            record->Set(f->first, className + ":" + std::to_string((*it)->GetRecordID()));
                            found = true;
                            break;
                        }
                    }
                }
                
                if(found == false) {
                    // Create a new data record
                    DataRecord* newRecord = new DataRecord();
                    newRecord->SetObject(obj);
                    
                    // Need to add the record in, serialize
                    obj->PreSerialize();
                    obj->Serialize(newRecord);
                    obj->PostSerialize();
                    
                    CreateRecords(newRecord);
                    
                    DataRecordType* drt = DataRecordType::GetType(className);
                    
                    // Create insert statement
                    query = "INSERT INTO " + className + " (";
                    std::string values = ") VALUES (";
                    
                    std::string newPrimaryKey = drt->GetPrimaryKey();
                    std::map<std::string, int> newFields = drt->GetKeys();
                    std::map<std::string, int>::iterator fi = newFields.begin();
                    query += fi->first;
                    values += "'" + newRecord->GetString(fi->first) + "'";
                    fi++;
                    
                    for(; fi != newFields.end(); fi++) {
                        std::string field = fi->first;
                        if(field.compare(newPrimaryKey) != 0) {
                            query += "," + field;
                            values += ",'" + newRecord->GetString(field) + "'";
                        }
                    }
                    
                    query += values + ")";
                    
                    if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to insert object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                        assert(false); // Probably need to break here so you don't keep repeating this
                    }
                    
                    unsigned int newID = (unsigned int)sqlite3_last_insert_rowid(m_handle);
                    if(newID == 0) {
                        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to insert object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                        assert(false); // Probably need to break here so you don't keep repeating this
                    }
                    
                    newRecord->SetRecordID(newID);
                    record->Set(f->first, className + ":" + std::to_string(newID));
                    
                    m_records[className].push_back(newRecord);
                }
            }
        }
    }
}

void SQLiteDataLoader::SaveRecords() {
    UpdateTables();
    
    // Insert records
    std::map<std::string, std::vector<DataRecord*>>::iterator i = m_records.begin();
    for(; i != m_records.end(); i++) {
        for(int j = (int)i->second.size() - 1; j >= 0; j--) {
            // Get our record out
            DataRecord* record = i->second[j];
            std::string query = "";
            
            // Get record type
            DataRecordType* type = record->GetType();
            
            // Serialize
            record->GetObject()->PreSerialize();
            record->GetObject()->Serialize(record);
            record->GetObject()->PostSerialize();
            
            // Create any sub-objects
            CreateRecords(record);
            
            // Get the primary key
            char primaryKeyID[10];
            sprintf(primaryKeyID, "%d", record->GetRecordID());
            
            // Process deletion
            if(record->IsDeleted()) {
                // Construct query
                query = "DELETE FROM " + i->first + " WHERE " + type->GetPrimaryKey() + " = ";
                query += primaryKeyID;
                
                if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                    Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to delete object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                // Remove from our records as well
                i->second.erase(std::remove(i->second.begin(), i->second.end(), record), i->second.end());
                
                continue;
            }
            
            // Get fields to update
            std::vector<std::string> pairs;
            std::string primaryKey = type->GetPrimaryKey();
            
            std::map<std::string, int> fields = type->GetKeys();
            std::map<std::string, int>::iterator f = fields.begin();
            
            // Process new records
            if(record->GetRecordID() == 0) {
                query = "INSERT INTO " + i->first + " (";
                std::string values = ") VALUES (";
                std::map<std::string, int>::iterator f = fields.begin();
                query += f->first;
                values += "'" + record->GetString(f->first) + "'";
                f++;
                
                for(; f != fields.end(); f++) {
                    std::string field = f->first;
                    if(field.compare(primaryKey) != 0) {
                        query += "," + field;
                        values += ",'" + record->GetString(field) + "'";
                    }
                }
                
                query += values + ")";
                
                if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                    Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to insert object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                unsigned int newID = (unsigned int)sqlite3_last_insert_rowid(m_handle);
                if(newID == 0) {
                    Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to insert object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                    assert(false); // Probably need to break here so you don't keep repeating this
                }
                
                record->SetRecordID(newID);
                continue;
            }
            
            // Update record
            query = "UPDATE " + i->first + " SET ";
            f = fields.begin();
            query += f->first + " = '" + record->GetString(f->first) + "'";
            f++;
            
            for(; f != fields.end(); f++) {
                std::string field = f->first;
                if(field.compare(primaryKey) != 0) {
                    query += "," + field + " = '" + record->GetString(field) + "'";
                }
            }
            
            query += " WHERE " + primaryKey + " = " + primaryKeyID;
            
            if(sqlite3_exec(m_handle, query.c_str(), 0, 0, 0) != 0) {
                Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to update object from SQLite.", (char*)sqlite3_errmsg(m_handle)));
                //assert(false); // Probably need to break here so you don't keep repeating this
            }
        }
    }
}
