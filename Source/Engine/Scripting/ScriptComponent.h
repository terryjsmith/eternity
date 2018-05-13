
#ifndef scriptcomponent_hpp
#define scriptcomponent_hpp

#include <eternity.h>
#include <Scripting/Script.h>
#include <Core/Component.h>
#include <Scripting/ScriptThread.h>
#include <v8.h>

class GIGA_API ScriptComponent : public Component {
    ScriptComponent();
    ~ScriptComponent() = default;
    
	/**
	 * Initialize from a source script
	 */
    void Initialize(Script* script);

	/**
	 * Call a function inside this component
	 */
	void CallFunction(std::string function, int argc, Variant** argv);

	/**
	 * Cached function for quick calling
	 */
	struct ScriptFunction {
		std::string funcName;
		v8::Persistent<v8::Function> func;
	};

protected:
	void SetGlobal(std::string name, Variant* value);
    
protected:
    // Our script
    Script* m_scriptSource;

	// The thread this script was created on
	ScriptThread* m_thread;

	// Our internal script object
	v8::Persistent<v8::Script, v8::CopyablePersistentTraits<v8::Script>> m_script;

	// Our internal context
	v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>> m_context;

	// List of functions from inside of the script
	std::vector<ScriptFunction*> m_functions;
};

#endif
