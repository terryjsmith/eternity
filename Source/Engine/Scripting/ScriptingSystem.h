
#ifndef scriptingsystem_h
#define scriptingsystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Scripting/ScriptCallbackHandler.h>
#include <Scripting/ScriptVariant.h>

class ScriptingSystem : public System {
public:
	ScriptingSystem() = default;
	~ScriptingSystem() = default;
    
    std::map<std::string, ScriptVariant*>& GetGlobals() { return m_globals; }

protected:
    // Registered globals
    std::map<std::string, ScriptVariant*> m_globals;
};

#endif
