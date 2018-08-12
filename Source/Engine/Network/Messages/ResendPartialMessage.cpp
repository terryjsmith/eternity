
#include <Network/Messages/ResendPartialMessage.h>
#include <IO/MemoryWriter.h>
#include <Network/Messages/NetworkMessageTypes.h>

ResendPartialMessage::~ResendPartialMessage() {
    std::vector<ResendRequestInfo*>::iterator i = m_packets.begin();
    for(; i != m_packets.end(); i++) {
        delete (*i);
    }
    
    m_packets.clear();
}

void ResendPartialMessage::AddPacket(int messageID, int chunkID) {
    ResendRequestInfo* info = new ResendRequestInfo();
    info->messageID = messageID;
    info->chunkID = chunkID;
    
    m_packets.push_back(info);
}

void ResendPartialMessage::OnSend() {
    m_envelope.type = NetworkMessages::idResendPartialMessage;
    
    int size = sizeof(uint32_t) + (sizeof(uint32_t) * 2 * m_packets.size());
    unsigned char* buffer = (unsigned char*)malloc(size);
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(buffer, size);
    
    uint32_t numPackets = (uint32_t)m_packets.size();
    writer->Write(&numPackets, sizeof(uint32_t));
    
    std::vector<ResendRequestInfo*>::iterator i = m_packets.begin();
    for(; i != m_packets.end(); i++) {
        writer->Write(&(*i)->messageID, sizeof(uint32_t));
        writer->Write(&(*i)->chunkID, sizeof(uint32_t));
    }
    
    SetPayload(buffer, size);
    delete writer;
}

void ResendPartialMessage::OnReceive() {
    
}
