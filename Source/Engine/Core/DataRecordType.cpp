
#include <Core/DataRecordType.h>

std::map<std::string, DataRecordType*> DataRecordType::m_types;

void DataRecordType::AddKey(std::string name, int type, bool editable) {
    m_keys[name] = type;

	DataRecordField* field = new DataRecordField;
	field->name = name;
	field->type = type;
	field->editable = editable;
	field->friendly_name = std::string();

	m_details[name] = field;
}

void DataRecordType::SetKeyNonEditable(std::string key) {
	m_details[key]->editable = false;
}

void DataRecordType::SetKeyFriendlyName(std::string key, std::string name) {
	std::map<std::string, DataRecordField*>::iterator it = m_details.find(key);
	GIGA_ASSERT(it != m_details.end(), "Key not found.");

	m_details[key]->friendly_name = name;
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

DataRecordType::DataRecordField* DataRecordType::GetKeyDetail(std::string name) {
	return(m_details[name]);
}

int DataRecordType::GetKeyType(std::string name) {
    std::map<std::string, int>::iterator it = m_keys.find(name);
    if(it != m_keys.end()) {
        return(it->second);
    }
    
    return(0);
}
