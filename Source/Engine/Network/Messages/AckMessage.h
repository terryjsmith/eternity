
#ifndef ackmessage_h
#define ackmessage_h

#include <eternity.h>
#include <Network/NetworkMessage.h>

/**
 * Ack response message
 */
class GIGA_API AckMessage : public NetworkMessage {
public:
    AckMessage() = default;
    ~AckMessage() = default;
    
    /**
     * Send a connection request (client)
     */
    void OnSend();
    
    /**
     * Receive a connection request and reply (server)
     */
    void OnReceive();
    
public:
    // Message ID
    uint32_t messageID;
};

#endif
