#ifndef command_h
#define command_h

#include <eternity.h>
#include <Core/Message.h>

enum {
    COMMAND_START = 1,
    COMMAND_END = 0
};

GIGA_CLASS()
class GIGA_API Command : public Message {
public:
	Command();
	Command(std::string type, bool start);
	~Command() = default;

	GIGA_CLASS_NAME("Command");

	/**
	 * Initialize a new command
	 */
	 // GIGA_FUNCTION() void Initialize(Variant** argv, int argc);

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
	GIGA_FUNCTION() int GetMessageType() { return m_message; }
	GIGA_FUNCTION() int GetType() { return m_type; }
    
protected:
    // Start or end?
    uint32_t m_type;

	// Command message
	uint32_t m_message;
    
    // The command ID
    uint32_t m_commandID;
    
	// Registered message types
    static std::map<std::string, uint32_t> m_messageTypes;
    
	// Last issued command ID
    static int m_lastCommandID;
};

#endif
