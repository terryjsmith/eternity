
#include <Network/Messages/AckMessage.h>
#include <Network/NetworkSystem.h>
#include <Core/Application.h>
#include <Network/Messages/NetworkMessageTypes.h>

void AckMessage::OnSend() {
    m_envelope.type = NetworkMessages::idAckMessage;
    
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t));
    memcpy(message, &messageID, sizeof(uint32_t));
    
    SetPayload(message, sizeof(uint32_t));
}

void AckMessage::OnReceive() {
    // Get message ID
    uint32_t messageID = (uint32_t)*m_payload;
    
    // Mark message as received
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    networkSystem->MarkReceived(m_envelope.session, messageID);
}
