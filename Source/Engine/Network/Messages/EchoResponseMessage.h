
#ifndef echoresponsemessage_h
#define echoresponsemessage_h

#include <eternity.h>
#include <Network/NetworkMessage.h>

/**
 * Server response to ping (syn) message
 */
class GIGA_API EchoResponseMessage : public NetworkMessage {
public:
    EchoResponseMessage() = default;
    ~EchoResponseMessage() = default;
    
    /**
     * Send a connection request (client)
     */
    void OnSend();
    
    /**
     * Receive a connection request and reply (server)
     */
    void OnReceive();
    
    /**
     * Set timestamp set by client
     */
    void SetClientTimestamp(timespec t) { m_clientTime = t; }
    
protected:
    // Client time (for calculating RTT)
    timespec m_clientTime;
};

#endif
