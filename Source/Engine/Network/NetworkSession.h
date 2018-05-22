
#ifndef networksession_hpp
#define networksession_hpp

#include <eternity.h>
#include <Network/UDPSocket.h>
#include <Network/NetworkMessage.h>

/**
 * Connection properties (shared between client and server)
 */
struct GIGA_API NetworkConnectionInfo {
    // Difference in timestamp between us and server (in seconds)
    float clientTimeDiff;
    
    // Current ping time (one way - in seconds)
    float pingTime;
    
    // Last few ping times
    float pingTimes[10];
    int pingIndex;
    
    // Client offset times
    float offsetTimes[10];
    int offsetIndex;
};

/**
 * A single session/connection from client to server with metadata
 */
class GIGA_API NetworkSession {
public:
    NetworkSession();
    ~NetworkSession();
    
    /**
     * Write a message to this session (including session specific info)
     */
    void Write(NetworkMessage* msg);
    
public:
    // Session identifier
    uint32_t sessionID;
    
    // Socket connection
    UDPSocket* socket;
    
    // Connection info
    NetworkConnectionInfo info;
    
    // The player entity of this player in the world
    int playerID;
    
    // Last ping time (in ticks)
    int lastPing;
    
    // Last command message processed
    int lastCommandMessage;
};

#endif
