
#ifndef scriptthread_h
#define scriptthread_h

#include <eternity.h>
#include <v8.h>

class GIGA_API ScriptThread {
public:
	ScriptThread() = default;
	~ScriptThread() = default;

	/**
	* Thread initialization
	*/
	void Initialize();

	/**
	* Thread shutdown
	*/
	void Shutdown();

	/**
	* Get isolate
	*/
	v8::Isolate* GetIsolate() { return m_isolate; }

	/**
	* Lock thread
	*/
	void Lock(ScriptThread* thread);

	/**
	* Check lock
	*/
	bool IsLocked();

	/**
	* Unlock
	*/
	void Unlock();

	/**
	* Get a callback implementation
	*/
	ScriptableObjectImpl* GetScriptableImpl(std::string name);

	/**
	* The currently executing script (so events can be proxied back in)
	*/
	void SetCurrentScript(ScriptComponent* component) { m_currentScript = component; }
	ScriptComponent* GetCurrentScript() { return m_currentScript; }

protected:
	// V8 isolated execution environment (own heap, stack, GC, etc.)
	v8::Isolate* m_isolate;
	v8::Locker* m_locker;

	// Scriptable object types initialized in this isolate
	std::vector<ScriptableObjectImpl*> m_impls;

	// Currently executing script component
	ScriptComponent* m_currentScript;

	// The thread which is currently locking this
	ScriptThread* m_currentLocker;
};
};

#endif