
#include <Scripting/ScriptCallbackHandler.h>
#include <Scripting/ScriptVariant.h>
#include <Core/MetaSystem.h>
#include <Core/Application.h>
#include <Scripting/ScriptThread.h>

void ScriptCallbackHandler::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    int* existing = (int*)isolate->GetData(1);
    
    if(existing) {
        return info.GetReturnValue().Set(info.This());
    }
    
	v8::HandleScope scope(info.GetIsolate());

	// Convert our callback info into SmartValue objects
	Variant** args = (Variant**)malloc(sizeof(Variant*) * info.Length());
	for (int i = 0; i < info.Length(); i++) {
		args[i] = new ScriptVariant(info[i]);
	}

	v8::Local<v8::Function> func = info.Callee();
	v8::String::Utf8Value funcName(func->GetName());

	// Get the script interface we're dealing with
	MetaSystem* metaSystem = GetSystem<MetaSystem>();

	// Create a new object
	GigaObject* obj = 0;
	if (info.Length() > 0) {
		CallableFunction ctr = metaSystem->FindFunction(*funcName, "New");
		GIGA_ASSERT(ctr != 0, "No constructors for object that takes parameters.");

		Variant* newobj = ctr(0, info.Length(), args);
		GIGA_ASSERT(newobj != 0, "Unable to create new object.");

		obj = newobj->AsObject();
	}
	else {
		obj = metaSystem->CreateObject(*funcName);
	}

	// Lock mutex on new object
	obj->LockMutex();

	// Wrap C++ object in JS packaging
	Wrap(obj, info.This());

	for (int i = 0; i < info.Length(); i++) {
		delete args[i];
	}
	free(args);

	// Unlock and return new object
	obj->UnlockMutex();
	return info.GetReturnValue().Set(info.This());
}

void ScriptCallbackHandler::HandleStaticFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
	// Turn our info array into a list of passable Variants
	int argc = info.Length();
	Variant** argv = (Variant**)malloc(sizeof(Variant*) * argc);
	for (int i = 0; i < argc; i++) {
		argv[i] = new ScriptVariant(info[i]);
	}

	// Get current thread out of isolate
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
	ScriptComponent* component = thread->GetCurrentScript();

	v8::Local<v8::Object> holder = info.This();
	v8::Local<v8::Function> funcHolder = holder.As<v8::Function>();
	v8::String::Utf8Value thisName(funcHolder->GetName());
	
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    ScriptVariant* retval = 0;
    
    v8::Local<v8::Function> func = info.Callee();
    v8::String::Utf8Value funcName(func->GetName());
    
    GigaObject* singleton = metaSystem->GetSingleton(*thisName);
    if(singleton) {
        singleton->LockMutex();
        retval = (ScriptVariant*)singleton->Call(*funcName, argc, argv);
        singleton->UnlockMutex();
    }
    else {
        CallableFunction func = metaSystem->FindFunction(*thisName, *funcName);
        retval = (ScriptVariant*)func(0, argc, argv);
    }
    
    v8::Local<v8::Value> val = retval->GetValue();
    info.GetReturnValue().Set(val);
    
    delete retval;

	// Clean up
	for (int i = 0; i < argc; i++) {
		delete argv[i];
		argv[i] = 0;
	}

	free(argv);
}

void ScriptCallbackHandler::HandleObjectFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
    // Unwrap our object
    GigaObject* jsobj = Unwrap(info.This());
    jsobj->LockMutex();
    
    // Turn our info array into a list of passable Variants
    int argc = info.Length();
    Variant** argv = (Variant**)malloc(sizeof(Variant*) * argc);
    for (int i = 0; i < argc; i++) {
        argv[i] = new ScriptVariant(info[i]);
    }
    
    // Get current thread out of isolate
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    ScriptComponent* component = thread->GetCurrentScript();
    
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    ScriptVariant* retval = 0;
    
    v8::Local<v8::Function> func = info.Callee();
    v8::String::Utf8Value funcName(func->GetName());
    
    std::string className = jsobj->GetGigaName();
    CallableFunction callable = metaSystem->FindFunction(className, *funcName);
    retval = (ScriptVariant*)callable(jsobj, argc, argv);
    
    v8::Local<v8::Value> val = retval->GetValue();
    info.GetReturnValue().Set(val);
    
    delete retval;
    
    // Clean up
    for (int i = 0; i < argc; i++) {
        delete argv[i];
        argv[i] = 0;
    }
    
    free(argv);
    jsobj->UnlockMutex();
}

void ScriptCallbackHandler::HandleObjectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
    // Unwrap our object
    GigaObject* jsobj = Unwrap(info.This());
    jsobj->LockMutex();
    
    // Get current thread out of isolate
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    ScriptComponent* component = thread->GetCurrentScript();
    
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    ScriptVariant* retval = 0;
    
    v8::String::Utf8Value propName(property);
    MetaSystem::GetterFunction getfunc = metaSystem->FindVariableGetFunction(jsobj->GetGigaName(), *propName);
    GIGA_ASSERT(getfunc != 0, "Get function not defined for variable.");
    
    ScriptVariant* value = (ScriptVariant*)(getfunc(jsobj));
    v8::Local<v8::Value> val = value->GetValue();
    info.GetReturnValue().Set(val);
    
    jsobj->UnlockMutex();
}

void ScriptCallbackHandler::HandleObjectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
    // Unwrap our object
    GigaObject* jsobj = Unwrap(info.This());
    jsobj->LockMutex();
    
    // Get current thread out of isolate
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    ScriptComponent* component = thread->GetCurrentScript();
    
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    ScriptVariant* retval = 0;
    
    v8::String::Utf8Value propName(property);
    MetaSystem::SetterFunction setfunc = metaSystem->FindVariableSetFunction(jsobj->GetGigaName(), *propName);
    GIGA_ASSERT(setfunc != 0, "Set function not defined for variable.");
    
    Variant* sv = new ScriptVariant(value);
    setfunc(jsobj, sv);
    
    delete sv;
    jsobj->UnlockMutex();
}
