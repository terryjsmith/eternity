#ifndef command_h
#define command_h

#include <eternity.h>
#include <Core/Message.h>

enum {
    COMMAND_START = 1,
    COMMAND_END
};

GIGA_CLASS()
class GIGA_API Command : public Message {
public:
    Command();
    ~Command() = default;
    
    GIGA_CLASS_NAME("Command");
    
    /**
     * Initialize a new command
     */
    // GIGA_FUNCTION() void Initialize(Variant** argv, int argc);
    
    /**
     * Record start/end
     */
    GIGA_FUNCTION() void Start();
    GIGA_FUNCTION() void End();
    
    /**
     * Register a new command type
     */
    GIGA_FUNCTION() static void RegisterCommandType(std::string name, int typeID);
    
    /**
     * Get current command ID
     */
    static int GetLastCommandID() { return m_lastCommandID; }
    
    /**
     * Getters
     */
    GIGA_FUNCTION() int GetType() { return m_type; }
    GIGA_FUNCTION() int GetEntityID() { return m_entityID; }
    GIGA_FUNCTION() int GetStart() { return m_start; }
    GIGA_FUNCTION() int GetEnd() { return m_end; }
    GIGA_FUNCTION() int GetSessionID() { return m_sessionID; }
    
protected:
    // The integer type (from the registration for serialization)
    uint32_t m_type;
    
    // The entity this command is occuring on
    uint32_t m_entityID;
    
    // The start time
    uint32_t m_start;
    
    // The end time
    uint32_t m_end;
    
    // The session ID
    uint32_t m_sessionID;
    
    // The command ID
    uint32_t m_commandID;
    
protected:
    struct CommandType {
        std::string name;
        uint32_t typeID;
    };
    
    static std::vector<CommandType*> m_commandTypes;
    
    static int m_lastCommandID;
};

#endif
