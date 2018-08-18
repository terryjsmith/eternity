
#ifndef networksystem_hpp
#define networksystem_hpp

#include <eternity.h>
#include <Core/System.h>
#include <Network/NetworkMessagePart.h>
#include <Network/NetworkSystemInfo.h>

#define NETWORK_MAX_PACKET_SIZE     8000
#define NETWORK_TICKS_PER_SECOND    20
#define NETWORK_ECHO_TICKS          10
#define NETWORK_ECHO_TIMEOUT        60
#define NETWORK_RESEND_MULTIPLE     2
#define NETWORK_RESEND_HISTORY      60

/**
 * Network system, handling sessions - can act as client or server
 */
GIGA_CLASS(Singleton)
class GIGA_API NetworkSystem : public System {
public:
    NetworkSystem();
    ~NetworkSystem();
    
    GIGA_CLASS_NAME("NetworkSystem");
    
    enum {
        NETWORK_SYSTEM_CLIENT = 1,
        NETWORK_SYSTEM_SERVER
    };
    
    /**
     * Initialize (set startup time)
     */
    void Initialize();
    
    /**
     * Connect to remote server (when connected as client)
     */
    GIGA_FUNCTION() void Connect(std::string host, int port);
    
    /**
     * Listen for new connections on a particular port (server)
     */
    void Listen(int port);
    
    /**
     * Send a message to all connected sessions
     */
    GIGA_FUNCTION() void Send(NetworkMessage* msg);
    
    /**
     * Send a message to a single session
     */
    GIGA_FUNCTION() void Send(int sessionID, NetworkMessage* msg);
    
    /**
     * Process/receive new messages
     */
    void Update(float delta);
    
    /**
     * Get current tick
     */
    int GetCurrentTick();
    
    /**
     * Get current offset time since start
     */
    float GetCurrentTickTime();
    
    /**
     * Get/set server startup time
     */
    timespec GetStartupTime() { return m_startupTime; }
    void SetStartupTime(timespec t) { m_startupTime = t; }
    
    /**
     * Add a session to our tracked sessions (server)
     */
    NetworkSession* FindSession(int sessionID, UDPSocket* socket = 0);
    
    /**
     * Remove a session
     */
    void RemoveSession(int sessionID);
    
    /**
     * Set a manual "tick offset" for replaying
     */
    void SetTick(int tick) { m_overrideTick = tick; }
    
    /**
     * Register a new message type (by class and unique ID)
     */
    template<typename T>
    void RegisterMessageType(int messageID) {
        // Make sure this type isn't already registered
        std::map<int, NetworkMessage*(*)()>::iterator i = m_messageTypes.find(messageID);
        GIGA_ASSERT(i == m_messageTypes.end(), "Message type ID already registered.");
        
        // Register it (by message ID for faster lookup on incoming packets)
        m_messageTypes[messageID] = &CreateMessage<T>;
    }
    
    /**
     * Mark a message as received (ack)
     */
    void MarkReceived(int sessionID, int messageID);
    
    /**
     * Get network system info
     */
    NetworkSystemInfo* GetSystemInfo() { return &m_info; }
    
    /**
     * Set connection info
     */
    GIGA_FUNCTION() void SetSessionAuth(std::string sessionCode);
    
protected:
    // Internal send function
    void Send(NetworkSession* session, NetworkMessage* msg);
    
protected:
    // Create a new message from a class type
    template<typename T> static NetworkMessage* CreateMessage() { return new T; }
    
protected:
    // Max socket ID we should poll for
    int m_maxSocketID;
    
    // Startup time
    timespec m_startupTime;
    
    // Mapping of registered network message types
    std::map<int, NetworkMessage*(*)()> m_messageTypes;
    
    // Client/server specific information
    NetworkSystemInfo m_info;
    
    // Session auth code
    std::string m_sessionCode;
    
    // Setup type
    int m_systemType;
    
    // Last message ID sent
    int m_lastMessageID;
    
    // Map of message IDs to incomplete message parts
    std::map<uint32_t, NetworkMessagePart*> m_partials;
    
    // Manual tick offset for replaying
    int m_overrideTick;
    
    // Network messages waiting for an ack response
    std::vector<NetworkMessage*> m_acks;
};


#endif
