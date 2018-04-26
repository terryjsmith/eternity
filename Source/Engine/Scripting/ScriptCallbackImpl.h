
#ifndef scriptobjectimpl_h
#define scriptobjectimpl_h

#include <eternity.h>
#include <v8.h>

class GIGA_API ScriptCallbackImpl {
public:
	ScriptCallbackImpl() = default;
	~ScriptCallbackImpl() = default;
	
	/**
	 * Create an implementation from an object type definition
	 */
	// void Create(ScriptableObjectType* type, ScriptThread* thread);

	/**
	* Called from JS to create a new version of our object to pass back
	*/
	static void New(const v8::FunctionCallbackInfo<v8::Value>& info);

	/**
	 * Wrap C++ object into JS object
	 */
	static inline void Wrap(GigaObject* obj, v8::Local<v8::Value> handle) {
		v8::Local<v8::Object> inthandle = handle.As<v8::Object>();
		assert(inthandle->InternalFieldCount() > 0);

		inthandle->SetAlignedPointerInInternalField(0, obj);
	}

	/**
	* Unwrap C++ object from JS object
	*/
	static inline GigaObject* Unwrap(v8::Local<v8::Object> handle) {
		assert(!handle.IsEmpty());
		assert(handle->InternalFieldCount() > 0);

		// Cast to ObjectWrap before casting to T.  A direct cast from void
		// to T won't work right when T has more than one base class.
		void* ptr = handle->GetAlignedPointerFromInternalField(0);
		GigaObject* wrap = static_cast<GigaObject*>(ptr);

		return(wrap);
	}

	/**
	* Handle static function callbacks (ie. Time.GetTime())
	*/
	static void HandleStaticFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

	/**
	* Handle object function callbacks (ie. this.DoSomething())
	*/
	static void HandleObjectFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

	/**
	* Handle object getters (call internal callbacks based on name)
	*/
	static void HandleObjectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

	/**
	* Handle object setters (call internal callbacks based on name)
	*/
	static void HandleObjectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

	/**
	* Handle static getter calls (call internal callback based on name)
	*/
	static void HandleStaticGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

	/**
	* Create a JavaScript object to pass back to V8
	*/
	v8::Local<v8::Object> CreateJSObject();

	struct ScriptObjectType {
		// Name
		std::string name;

		// The function template
		v8::Persistent<v8::FunctionTemplate, v8::CopyablePersistentTraits<v8::FunctionTemplate>> functionTemplate;

		// Our constructor to create new objects of this type
		v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> constructor;
	};

protected:
	std::map<std::string, ScriptObjectType*> m_types;
};

#endif