
#ifndef echorequestmessage_hpp
#define echorequestmessage_hpp

#include <Network/NetworkMessage.h>

/**
 * Ping from client to server
 */
class GIGA_API EchoRequestMessage : public NetworkMessage {
public:
    EchoRequestMessage() = default;
    ~EchoRequestMessage() = default;
    
    /**
     * Send a connection request (client)
     */
    void OnSend();
    
    /**
     * Receive a connection request and reply (server)
     */
    void OnReceive();
};

#endif
