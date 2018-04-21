
#ifndef error_h
#define error_h

#include <eternity.h>
#include <Core/Message.h>

/**
 * Generic error handling class - passed to message system for handling
 */
class GIGA_API Error : public Message {
public:
	Error(int level, std::string message) :
		m_level(level),
		m_message(message) { }

	Error(int level, std::string message, std::string details) :
		m_level(level),
		m_message(message),
		m_details(details) { }

	~Error() = default;

	GIGA_CLASS_NAME("Error");

	/**
	 * Getter functions
	 */
	int GetErrorLevel() { return m_level; }
	std::string GetErrorMsg() { return m_message; }
	std::string GetErrorDetails() { return m_details; }

	enum {
		ERROR_NONE = 0,
		ERROR_INFO,
		ERROR_DEBUG,
		ERROR_WARN,
		ERROR_FATAL
	};

protected:
	// Error level
	int m_level;

	// Error string
	std::string m_message;

	// Details (like a filename)
	std::string m_details;
};

#endif