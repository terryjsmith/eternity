
#include <Scripting/ScriptThread.h>
#include <Core/MetaSystem.h>
#include <Core/TimeSystem.h>
#include <Core/Application.h>
#include <Scripting/ScriptCallbackHandler.h>
#include <Scripting/ScriptingSystem.h>

ScriptThread::ScriptThread() {
    m_isolate = 0;
    m_locker = 0;
    m_currentLocker = 0;
    m_currentScript = 0;
}

void ScriptThread::Initialize() {
    // Create an isolate
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    m_isolate = v8::Isolate::New(create_params);
    
    Lock(this);
    m_isolate->SetData(0, this);
    
    // Get our systems
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    // Get all meta class objects
    std::vector<MetaSystem::RegisteredClass*> classes = metaSystem->GetRegisteredClasses();
    std::vector<MetaSystem::RegisteredClass*>::iterator it = classes.begin();
    
    // Add classes
    for(; it != classes.end(); it++) {
        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(m_isolate);
        
        // Create type
        ScriptObjectType* type = new ScriptObjectType();
        type->name = (*it)->name;
        
        // Create template
        v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(m_isolate, ScriptCallbackHandler::New);
        
        // Add functions
        std::map<std::string, MetaSystem::RegisteredFunction*>::iterator fi = (*it)->functions.begin();
        for (; fi != (*it)->functions.end(); fi++) {
            if (fi->second->isStatic == false) {
                tpl->InstanceTemplate()->Set(v8::String::NewFromUtf8(m_isolate, fi->second->name.c_str()), v8::FunctionTemplate::New(m_isolate, ScriptCallbackHandler::HandleObjectFunctionCallback));
            }
            else {
                tpl->Set(v8::String::NewFromUtf8(m_isolate, fi->second->name.c_str()), v8::FunctionTemplate::New(m_isolate, ScriptCallbackHandler::HandleStaticFunctionCallback));
            }
        }
        
        // Add variables
        std::map<std::string, MetaSystem::RegisteredVariable*>::iterator vi = (*it)->variables.begin();
        for (; vi != (*it)->variables.end(); vi++) {
            tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(m_isolate, vi->second->name.c_str()), ScriptCallbackHandler::HandleObjectGetter, ScriptCallbackHandler::HandleObjectSetter);
        }
        
        // End template
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        
        // Set our class name
        tpl->SetClassName(v8::String::NewFromUtf8(m_isolate, type->name.c_str()));
        
        type->functionTemplate.Reset(m_isolate, tpl);
        
        // Add to our internal list
        m_types[type->name] = type;
    }
    
    Unlock();
}

v8::Local<v8::Object> ScriptThread::CreateJSObject(std::string className) {
    // Find our type
    std::map<std::string, ScriptObjectType*>::iterator it = m_types.find(className);
    GIGA_ASSERT(it != m_types.end(), "Type name not found.");
    
    v8::EscapableHandleScope handle_scope(m_isolate);
    v8::Local<v8::FunctionTemplate> tpl = it->second->functionTemplate.Get(m_isolate);
    v8::Local<v8::Function> constructor = tpl->GetFunction();
    
    v8::Local<v8::Object> obj = constructor->NewInstance();
    return(handle_scope.Escape(obj));
}

v8::Local<v8::Object> ScriptThread::GetJSObject(GigaObject* obj) {
    v8::EscapableHandleScope handle_scope(m_isolate);
    
    // Check for a cached value
    auto it = m_cached.find(obj);
    if(it != m_cached.end()) {
        v8::Local<v8::Object> jsobj = it->second.Get(m_isolate);
        return(handle_scope.Escape(jsobj));
    }
    
    // Otherwise, create one and cache it
    int existing = 1;
    m_isolate->SetData(1, &existing);
    v8::Local<v8::Object> jsobj = this->CreateJSObject(obj->GetGigaName());
    ScriptCallbackHandler::Wrap(obj, jsobj);
    m_isolate->SetData(1, 0);
    
    v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> p;
    p.Reset(m_isolate, jsobj);
    
    m_cached[obj] = p;
    return(handle_scope.Escape(jsobj));
}

void ScriptThread::Shutdown() {
    if (m_isolate) {
        m_isolate->Dispose();
    }
}

void ScriptThread::Lock(ScriptThread* locker) {
    if(locker == 0) {
        locker = this;
    }
    
    if(m_locker && locker == m_currentLocker) {
        return;
    }
    
    while(v8::Locker::IsLocked(m_isolate)) {
        TimeSystem::Sleep(1);
    }
    
    GIGA_ASSERT(m_locker == 0, "Locker already exists.");
    
    m_locker = new v8::Locker(m_isolate);
    m_isolate->Enter();
    m_currentLocker = locker;
}

bool ScriptThread::IsLocked() {
    if(m_locker) {
        return(true);
    }
    
    return(false);
}

void ScriptThread::Unlock() {
    m_isolate->Exit();
    
    if(m_locker) {
        if(v8::Locker::IsLocked(m_isolate)) {
            delete m_locker;
        }
    }
    
    m_locker = 0;
    m_currentLocker = 0;
}

std::vector<ScriptThread::ScriptObjectType*> ScriptThread::GetScriptObjectTypes() {
	std::vector<ScriptThread::ScriptObjectType*> ret;
	std::map<std::string, ScriptThread::ScriptObjectType*>::iterator it = m_types.begin();
	for (; it != m_types.end(); it++) {
		ret.push_back(it->second);
	}

	return(ret);
}
