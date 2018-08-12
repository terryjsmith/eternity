
#ifndef resendmessagerequest_h
#define resendmessagerequest_h

#include <eternity.h>
#include <Network/NetworkMessage.h>

class GIGA_API ResendPartialMessage : public NetworkMessage {
public:
    ResendPartialMessage() = default;
    ~ResendPartialMessage();
    
    struct ResendRequestInfo {
        uint32_t messageID;
        uint32_t chunkID;
    };
    
    /**
     * Add missing packet details
     */
    void AddPacket(int messageID, int chunkID);
    
    /**
     * Compose message
     */
    void OnSend();
    
    /**
     * Process message
     */
    void OnReceive();
    
protected:
    // The packets and byte ranges we are re-requesting
    std::vector<ResendRequestInfo*> m_packets;
};

#endif
