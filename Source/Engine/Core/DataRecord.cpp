
#include <Core/DataRecord.h>

DataRecord::DataRecord() {
    m_recordID = 0;
    m_object = 0;
    m_type = 0;
    m_deserialized = false;
    m_deleted = false;
}

DataRecord::~DataRecord() {
    std::map<std::string, Variant*>::iterator it = m_values.begin();
    for(; it != m_values.end(); it++) {
        delete it->second;
    }
    
    m_values.clear();
}

Variant* DataRecord::Get(std::string key) {
    std::map<std::string, Variant*>::iterator it = m_values.find(key);
    if(it != m_values.end()) {
        return(it->second);
    }
    
    return(0);
}

std::string DataRecord::GetString(std::string key) {
    std::string ret;
    
    std::map<std::string, std::string>::iterator it = m_strValues.find(key);
    if(it != m_strValues.end()) {
        ret = it->second;
    }
    
    return(ret);
}

void DataRecord::Set(std::string key, Variant* value) {
    std::map<std::string, Variant*>::iterator it = m_values.find(key);
    if(it != m_values.end()) {
        delete it->second;
    }
    
    m_values[key] = value;
    if(value->IsObject() == false) {
        m_strValues[key] = value->ToString();
    }
}

void DataRecord::Set(std::string key, std::string value) {
    // Get record type
    DataRecordType* type = DataRecordType::GetType(m_object->GetGigaName());
    
    // Get column type
    int keyType = type->GetKeyType(key);
    
    std::map<std::string, Variant*>::iterator it = m_values.find(key);
    if(it != m_values.end()) {
        m_strValues[key] = value;
        it->second->FromString(value, keyType);
        return;
    }

    // Convert
    Variant* v = new Variant();
    v->FromString(value, keyType);
    m_values[key] = v;
    
    m_strValues[key] = value;
}

DataRecordType* DataRecord::GetType() {
    if(m_type == 0) {
        m_type = DataRecordType::GetType(m_object->GetGigaName());
    }
    
    return(m_type);
}
