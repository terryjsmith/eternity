
#ifndef entitysnapshotmessage_h
#define entitysnapshotmessage_h

#include <eternity.h>
#include <Network/NetworkMessage.h>

/**
 * Receive the state of entities that have component-level changes with the server
 */
class GIGA_API EntitySnapshotMessage : public NetworkMessage {
public:
    EntitySnapshotMessage() = default;
    ~EntitySnapshotMessage() = default;
    
    /**
     * Send current entity state (from server)
     */
    void OnSend();
    
    /**
     * Receive entity state (client)
     */
    void OnReceive();
    
    /**
     * Set entity payload
     */
    void SetEntityPayload(unsigned char* payload, int size);
};

#endif
