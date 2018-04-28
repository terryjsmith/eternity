
#include <Scripting/ScriptCallbackHandler.h>
#include <Scripting/ScriptVariant.h>
#include <Core/MetaSystem.h>
#include <Core/Application.h>

void ScriptCallbackHandler::New(const v8::FunctionCallbackInfo<v8::Value>& info) {
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
		GigaObject* obj = metaSystem->CreateObject(*funcName);
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

	// Get the script interface we're supposed to connect to
	ScriptingSystem* ss = GetSystem<ScriptingSystem>();

	v8::Local<v8::Object> holder = info.This();
	v8::Local<v8::Function> funcHolder = holder.As<v8::Function>();
	v8::String::Utf8Value thisName(funcHolder->GetName());
	ScriptableObjectType* interface = ss->GetScriptableObjectType(*thisName);

	if (interface->m_staticObject) {
		interface->m_staticObject->LockMutex();
	}

	// Iterate through the callback list, looking for a registered callback function
	std::vector<ScriptableObjectType::ScriptFunctionCallback*> funcList = interface->GetFunctionList();
	for (size_t i = 0; i < funcList.size(); i++) {
		v8::Local<v8::Function> func = info.Callee();
		v8::String::Utf8Value funcName(func->GetName());
		if (funcList[i]->funcName == std::string(*funcName)) {
			if (funcList[i]->func != 0) {
				ScriptableVariant* scrval = 0;
				if (funcList[i]->isComponent == false) {
					scrval = (ScriptableVariant*)(funcList[i]->func(0, argc, argv));
				}
				else {
					scrval = (ScriptableVariant*)(funcList[i]->func2(component, 0, argc, argv));
				}
				v8::Local<v8::Value> val = scrval->GetValue();
				info.GetReturnValue().Set(val);
				delete scrval;

				break;
			}
		}
	}

	// Clean up
	for (int i = 0; i < argc; i++) {
		delete argv[i];
		argv[i] = 0;
	}

	if (interface->m_staticObject) {
		interface->m_staticObject->UnlockMutex();
	}

	free(argv);
}