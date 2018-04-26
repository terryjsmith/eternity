
#ifndef scriptobject_h
#define scriptobject_h

#include <eternity.h>
#include <Core/GigaObject.h>
#include <v8.h>

class ScriptThread;

class GIGA_API ScriptObject {
public:
	ScriptObject() = default;
	~ScriptObject() = default;

	/**
	* Return this object as a Javascript object
	*/
	v8::Local<v8::Object> GetJSObject();

	/**
	 * Get object itself
	 */
	GigaObject* GetObject() { return m_obj; }

protected:
	GigaObject* m_obj;

	// Cached JS variables for various threads
	v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> m_var;
};

#endif