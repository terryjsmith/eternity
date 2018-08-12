
#ifndef networkmessagepart_h
#define networkmessagepart_h

#include <eternity.h>
#include <Network/NetworkSession.h>
#include <Network/NetworkMessage.h>

/**
 * Partial messages
 */
class GIGA_API NetworkMessagePart {
public:
    NetworkMessagePart();
    ~NetworkMessagePart();
    
public:
    // Master message; all concatenated parts
    NetworkMessage* master;
    
    // Parts of the message (for tracking)
    std::vector<NetworkMessage*> parts;
    
    // Last tick we received message in
    uint32_t lastTick;
    
    // Network session this is attached to
    NetworkSession* session;
    
    // Whether this message has already been processed (in case of receiving partials twice)
    bool processed;
};

#endif
