
#ifndef scriptingsystem_h
#define scriptingsystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Scripting/ScriptCallbackHandler.h>

class ScriptingSystem : public System {
public:
	ScriptingSystem();
	~ScriptingSystem() = default;

protected:
	// Script callback handler for events/call on this thread
	ScriptCallbackHandler * m_handler;
};

#endif