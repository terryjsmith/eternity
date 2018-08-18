
#include <Network/Messages/HelloMessage.h>
#include <Network/Messages/NetworkMessageTypes.h>
#include <Network/NetworkSystem.h>
#include <Core/Application.h>
#include <IO/DataLoader.h>
#include <Core/Error.h>
#include <IO/MemoryReader.h>
#include <IO/MemoryWriter.h>

void HelloMessage::OnSend() {
    // Set payload
    if(m_payload) {
        free(m_payload);
        m_payload = 0;
    }
    
    uint32_t size = sessionCode.size();
    unsigned char* payload = (unsigned char*)malloc(sizeof(uint32_t) + size);
    
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(payload, sizeof(uint32_t) + size);
    
    writer->Write(&size, sizeof(uint32_t));
    writer->Write((char*)sessionCode.data(), size);
    delete writer;
    
    SetPayload(payload, writer->GetPosition());
    
    // Nothing to put here, payload set by SetEntityPayload
    m_envelope.type = NetworkMessages::idHelloMessage;
    
    delete writer;
    free(payload);
}

void HelloMessage::OnReceive() {
    // Check if we can make the server authoritative again
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    NetworkSession* session = networkSystem->FindSession(m_envelope.session);
    
    // Read out data
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(m_payload, m_envelope.bytes);
    
    uint32_t size = 0;
    reader->Read(&size, sizeof(uint32_t));
    
    sessionCode.resize(size);
    reader->Read((char*)sessionCode.data(), size);
    
    delete reader;
    
    // Validate session
    DataLoader* loader = GetAppService<DataLoader>();
    std::map<std::string, std::string> args;
    args["sessionCode"] = sessionCode;
    std::vector<DataRecord*> records = loader->GetRecords("Session", args);
    
    if(records.size() == 0) {
        Message::Broadcast(new Error(Error::ERROR_INFO, "Invalid session ID", sessionCode));
        return;
    }
    
    if(records[0]->Get("sessionID")->AsInt() != m_envelope.session) {
        Message::Broadcast(new Error(Error::ERROR_INFO, "Mis-matched session ID", sessionCode));
        return;
    }
    
    // Set player ID
    session->playerID = records[0]->Get("playerID")->AsInt();
    delete records[0];
}
