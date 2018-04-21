
#include <IO/Command.h>
#include <Core/Message.h>
#include <Core/TimeSystem.h>
#include <Core/Application.h>
#include <Core/Variant.h>

std::vector<Command::CommandType*> Command::m_commandTypes;
int Command::m_lastCommandID = 0;

Command::Command() {
    m_type = 0;
    m_start = 0;
    m_end = 0;
    m_entityID = 0;
    m_sessionID = 0;
    m_commandID = ++m_lastCommandID;
}

void Command::RegisterCommandType(std::string name, int typeID) {
    if (m_commandTypes.size()) {
        for (int i = (int)m_commandTypes.size() - 1; i >= 0; i--) {
            if (m_commandTypes[i]->name == name || m_commandTypes[i]->typeID == typeID) {
                GIGA_ASSERT(false, "Command type name or typeID already registered.");
                return;
            }
        }
    }
    
    CommandType* type = new CommandType;
    type->name = name;
    type->typeID = typeID;
    m_commandTypes.push_back(type);
}

void Command::Start() {
    TimeSystem* timeSystem = GetSystem<TimeSystem>();
    int tick = timeSystem->GetCurrentTick();
    
    m_start = tick;
    Message::Broadcast(this);
}

void Command::End() {
    TimeSystem* timeSystem = GetSystem<TimeSystem>();
    int tick = timeSystem->GetCurrentTick();
    
    m_end = tick;
    Message::Broadcast(this);
}
