
#include <Network/NetworkMessage.h>
#include <IO/MemoryReader.h>
#include <IO/MemoryWriter.h>

NetworkMessage::NetworkMessage() {
    memset(&m_envelope, 0, sizeof(NetworkEnvelope));
    m_payload = 0;
    m_payloadOffset = 0;
}

NetworkMessage::~NetworkMessage() {
    if(m_payload) {
        free(m_payload);
        m_payload = 0;
    }
}

void NetworkMessage::Initialize(unsigned char* buffer, int size) {
    // Read in the message type (first thing in envelope)
    int offset = 0;
    
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(buffer, size);
    
    reader->Read(&m_envelope.type, sizeof(uint16_t));
    reader->Read(&m_envelope.session, sizeof(int32_t));
    reader->Read(&m_envelope.tick, sizeof(int32_t));
    reader->Read(&m_envelope.id, sizeof(int32_t));
    reader->Read(&m_envelope.flags, sizeof(uint16_t));
    reader->Read(&m_envelope.bytes, sizeof(int32_t));
    reader->Read(&m_envelope.chunkID, sizeof(uint16_t));
    reader->Read(&m_envelope.end, sizeof(uint16_t));
    reader->Read(&m_envelope.lastCmd, sizeof(int32_t));
    
    // Otherwise, we have a message to process, read in the rest
    m_payload = (unsigned char*)malloc(m_envelope.bytes);
    reader->Read(m_payload, m_envelope.bytes);
    
    delete reader;
}

void NetworkMessage::Append(unsigned char* data, int start, int size) {
    GIGA_ASSERT(start + size < m_envelope.bytes, "Buffer overflow.");
    memcpy(m_payload + start, data, size);
}

unsigned char* NetworkMessage::GetPayload(int &size) {
    size = sizeof(NetworkEnvelope) + m_envelope.bytes;
    unsigned char* buffer = (unsigned char*)malloc(size);
    
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(buffer, size);
    
    // Write the message type (first thing in envelope)
    int offset = 0;
    
    writer->Write(&m_envelope.type, sizeof(int16_t));
    writer->Write(&m_envelope.session, sizeof(uint32_t));
    writer->Write(&m_envelope.tick, sizeof(uint32_t));
    writer->Write(&m_envelope.id, sizeof(uint32_t));
    writer->Write(&m_envelope.flags, sizeof(int16_t));
    writer->Write(&m_envelope.bytes, sizeof(uint32_t));
    writer->Write(&m_envelope.chunkID, sizeof(int16_t));
    writer->Write(&m_envelope.end, sizeof(int16_t));
    writer->Write(&m_envelope.lastCmd, sizeof(int32_t));
    writer->Write(m_payload, m_envelope.bytes);
    
    delete writer;
    return(buffer);
}

void NetworkMessage::ReadPayload(void* bytes, int size) {
    // printf("Bytes: %d, offset: %d, reading: %d\n", m_envelope.bytes, m_payloadOffset, size);
    GIGA_ASSERT(m_payloadOffset + size <= m_envelope.bytes, "Cannot read more than size of packet.");
    
    memcpy(bytes, m_payload + m_payloadOffset, size);
    m_payloadOffset += size;
}

void NetworkMessage::SetPayload(unsigned char* bytes, int size) {
    if(m_payload) {
        free(m_payload);
    }
    
    m_payload = (unsigned char*)malloc(size);
    memcpy(m_payload, bytes, size);
    m_envelope.bytes = size;
}

void NetworkMessage::Copy(NetworkMessage* copy) {
    memcpy(&copy->m_envelope, &m_envelope, sizeof(NetworkEnvelope));
    
    unsigned char* payload = (unsigned char*)malloc(m_envelope.bytes);
    memcpy(payload, m_payload, m_envelope.bytes);
    copy->SetPayload(payload, m_envelope.bytes);
}
