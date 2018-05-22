
#ifndef networkmessage_hpp
#define networkmessage_hpp

#include <eternity.h>

/**
 * Base class for other message types
 */
class GIGA_API NetworkMessage {
public:
    NetworkMessage();
    ~NetworkMessage();
    
    enum Flags {
        FLAG_ACK = 1,
    };
    
    /**
     * A network message header/envelope
     */
    struct NetworkEnvelope {
        uint32_t tick;
        uint32_t id;
        uint16_t type;
        uint32_t session;
        uint32_t lastCmd;
        uint32_t bytes;
        uint16_t chunkID;
        uint16_t end;
        uint16_t flags;
    };
    
    /**
     * Read a message in from a packet
     */
    void Initialize(unsigned char* buffer, int size);
    
    /**
     * Append new data to an existing buffer
     */
    void Append(unsigned char* data, int start, int size);
    
    /**
     * Parse an existing message out of the payload (called on receive)
     */
    virtual void OnReceive() = 0;
    
    /**
     * Serialize a message into the payload (called on send)
     */
    virtual void OnSend() = 0;
    
    /**
     * A callback for when the message is acked back
     */
    virtual void OnAck() { }
    
    /**
     * Get a packet with envelope + payload to be sent
     */
    unsigned char* GetPayload(int& size);
    
    /**
     * Get a part of the payload
     */
    unsigned char* GetRawPayload() { return m_payload; }
    
    /**
     * Get our envelope
     */
    NetworkEnvelope* GetEnvelope() { return &m_envelope; }
    
    /**
     * Does this message require an ack?
     */
    void SetFlags(int flags) { m_envelope.flags = flags; }
    int GetFlags() { return m_envelope.flags; }
    
    /**
     * Helper function to set payload (sets envelope size)
     */
    void SetPayload(unsigned char* bytes, int size);
    
    /**
     * Create a copy of the message
     */
    void Copy(NetworkMessage* copy);
    
protected:
    /**
     * Helper function to go through payload, reading bytes (keeps track of offset)
     */
    void ReadPayload(void* bytes, int size);
    
protected:
    // The envelope/header
    NetworkEnvelope m_envelope;
    
    // The payload (data)
    unsigned char* m_payload;
    
private:
    // Current payload offset (for ReadPayload function)
    unsigned int m_payloadOffset;
};

#endif
