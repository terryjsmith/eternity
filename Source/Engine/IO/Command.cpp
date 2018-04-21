
#include <IO/Command.h>
#include <Core/Message.h>
#include <Core/TimeSystem.h>
#include <Core/Application.h>
#include <Core/Variant.h>

std::map<std::string, uint32_t> Command::m_messageTypes;
int Command::m_lastCommandID = 0;

Command::Command() {
	m_type = 0;
	m_message = 0;
	m_commandID = ++m_lastCommandID;
}

Command::Command(std::string type, bool start) {
	std::map<std::string, uint32_t>::iterator it = m_messageTypes.find(type);
	if (it == m_messageTypes.end()) {
		GIGA_ASSERT(false, "Unregistered command type.");
	}

    m_type = start;
	m_message = it->second;
    m_commandID = ++m_lastCommandID;
}

void Command::RegisterCommandType(std::string name, int typeID) {
	std::map<std::string, uint32_t>::iterator it = m_messageTypes.find(name);
    if (it != m_messageTypes.end()) {
		GIGA_ASSERT(false, "Command type name or typeID already registered.");
    }
    
	m_messageTypes[name] = typeID;
}
