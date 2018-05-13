
#ifndef scriptthread_h
#define scriptthread_h

#include <eternity.h>
#include <v8.h>
#include <Core/GigaObject.h>

class ScriptComponent;

class GIGA_API ScriptThread {
public:
	ScriptThread();
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
	* The currently executing script (so events can be proxied back in)
	*/
	void SetCurrentScript(ScriptComponent* component) { m_currentScript = component; }
	ScriptComponent* GetCurrentScript() { return m_currentScript; }
    
    /**
     * Create a JS object of a specific class name
     */
    v8::Local<v8::Object> CreateJSObject(std::string className);
    
    /**
     * Check for an existing cached object or create one
     */
    v8::Local<v8::Object> GetJSObject(GigaObject* obj);
    
    struct ScriptObjectType {
        // Name
        std::string name;
        
        // The function template
        v8::Persistent<v8::FunctionTemplate, v8::CopyablePersistentTraits<v8::FunctionTemplate>> functionTemplate;
    };
    
protected:
    // Registered script object types
    std::map<std::string, ScriptObjectType*> m_types;
    
    // Cache of objects
    std::map<GigaObject*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>> m_cached;

	// V8 isolated execution environment (own heap, stack, GC, etc.)
	v8::Isolate* m_isolate;
	v8::Locker* m_locker;

	// Currently executing script component
	ScriptComponent* m_currentScript;

	// The thread which is currently locking this
	ScriptThread* m_currentLocker;
};

#endif
