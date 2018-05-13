
#include <Scripting/ScriptComponent.h>
#include <Scripting/ScriptingSystem.h>
#include <Core/Application.h>
#include <Core/Error.h>

ScriptComponent::ScriptComponent() {
    m_scriptSource = 0;
}

void ScriptComponent::Initialize(Script* script) {
	v8::Isolate* isolate = m_thread->GetIsolate();

	m_thread = (ScriptThread*)isolate->GetData(0);
	m_scriptSource = script;

	m_thread->SetCurrentScript(this);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
    v8::Local<v8::ObjectTemplate> global;
    v8::Local<v8::Context> context;
    
    // Create a global object template
    global = v8::ObjectTemplate::New(isolate);
    
    // Get our context
    context = v8::Context::New(isolate, NULL, global);
    m_context.Reset(isolate, context);
    
    context->Enter();

	// Get our global object space and start adding stuff to it
	v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();

	// Get our systems
	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();

	// Add globals
	std::map<std::string, ScriptVariant*> globals = scriptingSystem->GetGlobals();
	for (std::map<std::string, ScriptVariant*>::iterator i = globals.begin(); i != globals.end(); i++) {
		globalSpace->Set(v8::String::NewFromUtf8(isolate, i->first.c_str()), i->second->GetValue());
	}

	// Add classes
	std::vector<ScriptThread::ScriptObjectType*> types = m_thread->GetScriptObjectTypes();
	std::vector<ScriptThread::ScriptObjectType*>::iterator ti = types.begin();
	for (; ti != types.end(); ti++) {
		// Inject this type name into V8
		v8::Local<v8::FunctionTemplate> tpl = (*ti)->functionTemplate.Get(isolate);
		v8::Local<v8::Object> global = isolate->GetCurrentContext()->Global();

		global->Set(v8::String::NewFromUtf8(isolate, (*ti)->name.c_str()), tpl->GetFunction());
	}

	// Copy source
	std::string source = m_scriptSource->GetResource()->GetString();
	v8::Local<v8::String> scriptSrc = v8::String::NewFromUtf8(isolate, source.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

	v8::Local<v8::Script> vscript;
	if (v8::Script::Compile(context, scriptSrc).ToLocal(&vscript) == false) {
		v8::String::Utf8Value error(try_catch.Exception());
		Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to compile script file " + m_scriptSource->GetResource()->filename, *error));

		m_thread->SetCurrentScript(0);
		return;
	}

	// Save script
	m_script.Reset(isolate, vscript);

	v8::Local<v8::Value> result;
	if (vscript->Run(context).ToLocal(&result) == false) {
		v8::String::Utf8Value error(try_catch.Exception());
		Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to execute script file " + m_scriptSource->GetResource()->filename, *error));

		m_thread->SetCurrentScript(0);
		return;
	}

	// Get the init function
	v8::Local<v8::Value> value = globalSpace->Get(v8::String::NewFromUtf8(isolate, "Init"));
	if (value->IsFunction()) {
		if (this->m_parent) {
			Variant* parent = new Variant(this->m_parent);
			SetGlobal("GameObject", parent);
		}

		m_thread->SetCurrentScript(this);

		// Call the Init function if there is one
		v8::Local<v8::Function> initFunc = value.As<v8::Function>();
		result = initFunc->Call(globalSpace, 0, 0);

		// Attempt to get an error message (need a better way to do this)
		v8::String::Utf8Value error(try_catch.Exception());
		if (*error) {
			Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to execute init function " + m_scriptSource->GetResource()->filename, *error));
		}
	}

	// Loop through and find any global variable names
	v8::Local<v8::Array> vars = globalSpace->GetPropertyNames(context).ToLocalChecked();
	int sizearr = vars->Length();
	for (int i = 0; i < sizearr; i++) {
		v8::Local<v8::Value> var = vars->Get(i);

		v8::String::Utf8Value name(var->ToString(isolate));
		std::string typeStr = *name;

		v8::Local<v8::Value> actual = globalSpace->Get(context, v8::String::NewFromUtf8(isolate, typeStr.c_str())).ToLocalChecked();

		v8::String::Utf8Value type(actual->TypeOf(isolate));
		typeStr = *type;

		if (actual->IsFunction()) {
			v8::Local<v8::Function> func = actual.As<v8::Function>();
			v8::String::Utf8Value funcName(func->GetName());

			ScriptFunction* scriptFunc = new ScriptFunction();
			scriptFunc->func.Reset(isolate, func);
			scriptFunc->funcName = *funcName;

			m_functions.push_back(scriptFunc);
		}
	}

	context->Exit();
}

void ScriptComponent::SetGlobal(std::string name, Variant* value) {
	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
	m_thread->SetCurrentScript(this);

	v8::Isolate* isolate = m_thread->GetIsolate();

	// Create a stack-allocated handle scope.
	v8::HandleScope handle_scope(isolate);

	// Reset our current local context
	v8::Local<v8::Context> context = m_context.Get(isolate);
	context->Enter();

	// Catch any errors the script might throw
	v8::TryCatch try_catch(isolate);

	// Get into our global namespace
	v8::Local<v8::Object> globalSpace = context->Global();

	ScriptVariant* sv = (ScriptVariant*)value;
	globalSpace->Set(v8::String::NewFromUtf8(isolate, name.c_str()), sv->GetValue());

	// Exit
	context->Exit();
	m_thread->SetCurrentScript(0);
}