
#include <Network/Messages/CommandMessage.h>
#include <IO/MemoryReader.h>
#include <IO/MemoryWriter.h>
#include <Network/NetworkSystem.h>
#include <Network/ReplicationSystem.h>
#include <Core/Application.h>
#include <Network/Messages/NetworkMessageTypes.h>

CommandMessage::CommandMessage() {
    m_command = 0;
    m_envelope.flags |= NetworkMessage::Flags::FLAG_ACK;
    m_envelope.type = NetworkMessages::idCommandMessage;
}

void CommandMessage::OnSend() {
    GIGA_ASSERT(m_command != 0, "Command object not set.");
    
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 4);
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(message, sizeof(uint32_t) * 4);
    
    uint32_t type = m_command->GetType();
    uint32_t state = m_command->GetState();
    uint32_t entityID = m_command->GetEntityID();
    uint32_t commandID = m_command->GetCommandID();
    
    writer->Write(&type, sizeof(uint32_t));
    writer->Write(&state, sizeof(uint32_t));
    writer->Write(&entityID, sizeof(uint32_t));
    writer->Write(&commandID, sizeof(uint32_t));
    
    SetPayload(message, sizeof(uint32_t) * 4);
}

void CommandMessage::OnReceive() {
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(m_payload, m_envelope.bytes);
    
    uint32_t type, entityID, state, commandID;
    reader->Read(&type, sizeof(uint32_t));
    reader->Read(&state, sizeof(uint32_t));
    reader->Read(&entityID, sizeof(uint32_t));
    reader->Read(&commandID, sizeof(uint32_t));
    
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    NetworkSession* session = networkSystem->FindSession(m_envelope.session);
    int tick = networkSystem->GetCurrentTick();
    
    // Don't let the player tell us which entity
    entityID = session->playerID;
    
    int start, end;
    if (state == Command::COMMAND_START) {
        start = tick - floor(NETWORK_TICKS_PER_SECOND * session->info.pingTime) - NETWORK_SNAPSHOT_RENDER_LAG;
        printf("Current tick: %d, setting start to %d\n", tick, start);
    }
    else {
        end = tick - floor(NETWORK_TICKS_PER_SECOND * session->info.pingTime) - NETWORK_SNAPSHOT_RENDER_LAG;
        printf("Current tick: %d, setting end to %d\n", tick, end);
    }
    
    delete reader;
    
    // Make sure the replication system has been run for this tick
    ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
    
    // If this is a start command, make sure the end hasn't already been received
    if (end == 0) {
        // Find the start
        Command* endCmd = replicationSystem->GetCommand(commandID);
        if (endCmd) {
            endCmd->End();
            return;
        }
    }
    
    // Save command
    Command* command = new Command(state, type, entityID, commandID);
    
    replicationSystem->AddCommand(m_envelope.tick, m_envelope.session, command);
}

