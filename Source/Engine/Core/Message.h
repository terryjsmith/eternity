
#ifndef message_h
#define message_h

#include <eternity.h>
#include <Core/GigaObject.h>

/**
 * Message base class (for passing messages around)
 */
class GIGA_API Message : public GigaObject {
public:
	virtual ~Message() = default;

	static void Broadcast(Message* message, bool synchronized = false);

protected:
	// No direct creation
	Message();

public:
	uint32_t type;
};

#endif