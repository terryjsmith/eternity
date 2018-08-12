
#ifndef networksysteminfo_h
#define networksysteminfo_h

#include <eternity.h>
#include <Network/NetworkSession.h>
#include <Network/UDPSocket.h>

/**
 * Info specific to acting as a client
 */
struct GIGA_API NetworkClientInfo {
    // Time offset to server
    timespec timeOffset;
    
    // Our socket descriptor to check on
    fd_set sockets;
    
    // Our network session with the server
    NetworkSession* session;
    
    // Session ID (set before session object is created)
    int sessionID;
    
    // Player entity ID
    int playerID;
};

/**
 * Info specific to acting as a server
 */
struct GIGA_API NetworkServerInfo {
    // Our set of sockets to check
    fd_set listenSocket;
    
    // Listening socket (server)
    UDPSocket* socket;
    
    // Networking sessions tracked by server
    std::vector<NetworkSession*> sessions;
};

union NetworkSystemInfo {
    NetworkClientInfo* client_info;
    NetworkServerInfo* server_info;
};

#endif
