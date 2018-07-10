
#include <IO/DataLoader.h>

DataLoader::~DataLoader() {
    std::map<std::string, std::vector<DataRecord*>>::iterator it = m_records.begin();
    for(; it != m_records.end(); it++) {
        std::vector<DataRecord*>::iterator ri = it->second.begin();
        for(; ri != it->second.end(); ri++) {
            delete (*ri);
        }
        
        it->second.clear();
    }
    
    m_records.clear();
}

void DataLoader::AddRecord(GigaObject *record) {
    DataRecord* sr = new DataRecord();
    sr->SetRecordID(0);
    sr->SetObject(record);
    
    m_records[record->GetGigaName()].push_back(sr);
}
