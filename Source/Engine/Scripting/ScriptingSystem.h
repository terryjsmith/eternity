
#ifndef scriptingsystem_h
#define scriptingsystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Scripting/ScriptCallbackHandler.h>
#include <Scripting/ScriptVariant.h>

class ScriptingSystem : public System {
public:
	ScriptingSystem();
	~ScriptingSystem() = default;
    
    std::map<std::string, ScriptVariant*>& GetGlobals() { return m_globals; }

protected:
	// Script callback handler for events/call on this thread
	ScriptCallbackHandler * m_handler;
    
    // Registered globals
    std::map<std::string, ScriptVariant*> m_globals;
};

#endif
