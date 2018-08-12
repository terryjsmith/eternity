
#include <IO/Command.h>
#include <Core/Message.h>
#include <Core/TimeSystem.h>
#include <Core/Application.h>
#include <Core/Variant.h>

std::map<std::string, uint32_t> Command::m_messageTypes;
int Command::m_lastCommandID = 0;
std::vector<Command*> Command::m_commands;

Command::Command() {
	m_state = 0;
	m_message = 0;
	m_commandID = ++m_lastCommandID;
    m_entityID = 0;
}

Command::Command(uint32_t state, uint32_t type, uint32_t entityID, uint32_t commandID) {
    m_state = state;
    m_message = type;
    m_commandID = commandID == 0 ? ++m_lastCommandID : commandID;
    m_entityID = entityID;
}

Command* Command::Start(std::string type, uint32_t entityID) {
    std::map<std::string, uint32_t>::iterator it = m_messageTypes.find(type);
    if (it == m_messageTypes.end()) {
        GIGA_ASSERT(false, "Unregistered command type.");
    }
    
    Command* c = new Command();
    c->m_state = Command::COMMAND_START;
    c->m_message = it->second;
    c->m_commandID = ++m_lastCommandID;
    c->m_entityID = entityID;
    
    m_commands.push_back(c);
    
    Message::Broadcast(c);
    
    return(c);
}

Command* Command::End(std::string type, uint32_t entityID) {
    std::map<std::string, uint32_t>::iterator it = m_messageTypes.find(type);
    if (it == m_messageTypes.end()) {
        GIGA_ASSERT(false, "Unregistered command type.");
    }
    
    std::vector<Command*>::iterator ci = m_commands.begin();
    for(; ci != m_commands.end(); ci++) {
        if((*ci)->GetEntityID() == entityID && (*ci)->GetType() == it->second) {
            (*ci)->m_state = Command::COMMAND_END;
            Message::Broadcast(*ci);
            
            return(*ci);
        }
    }
    
    GIGA_ASSERT(false, "No open command.");
    return(0);
}

void Command::End() {
    m_state = Command::COMMAND_END;
    Message::Broadcast(this);
}

void Command::RegisterCommandType(std::string name, int typeID) {
	std::map<std::string, uint32_t>::iterator it = m_messageTypes.find(name);
    if (it != m_messageTypes.end()) {
		GIGA_ASSERT(false, "Command type name or typeID already registered.");
    }
    
	m_messageTypes[name] = typeID;
}
