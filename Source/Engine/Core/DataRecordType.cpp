
#include <Core/DataRecordType.h>

std::map<std::string, DataRecordType*> DataRecordType::m_types;

void DataRecordType::AddKey(std::string name, int type) {
    m_keys[name] = type;
}

void DataRecordType::Register(std::string name, DataRecordType* type) {
    m_types[name] = type;
}

DataRecordType* DataRecordType::GetType(std::string name) {
    std::map<std::string, DataRecordType*>::iterator it = m_types.find(name);
    if(it != m_types.end()) {
        return(it->second);
    }
    
    return(0);
}

int DataRecordType::GetKeyType(std::string name) {
    std::map<std::string, int>::iterator it = m_keys.find(name);
    if(it != m_keys.end()) {
        return(it->second);
    }
    
    return(0);
}
