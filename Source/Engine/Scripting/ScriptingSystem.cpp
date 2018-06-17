
#include <Scripting/ScriptingSystem.h>
#include <libplatform/libplatform.h>
#include <Core/Application.h>
#include <Core/MessageSystem.h>
#include <Core/Entity.h>

#ifdef WIN32
#include <direct.h>
#define getcwd	_getcwd
#endif

void ScriptingSystem::Initialize() {
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    
    m_platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(m_platform);
    
    char cwd[1000];
    getcwd(cwd, 1000);
    std::string file = cwd + std::string("/Resources/");
    
    v8::V8::InitializeExternalStartupData(file.c_str());
    
    ScriptThread::Initialize();
}

void ScriptingSystem::SetGlobal(std::string name, Variant* value) {
    m_globals[name] = (ScriptVariant*)value;
    
    // Add to any existing script components
    std::vector<ScriptComponent*>::iterator i = m_components.begin();
    for (i; i != m_components.end(); i++) {
        (*i)->SetGlobal(name, value);
    }
}

void ScriptingSystem::Update(float delta) {
    Variant* d = new Variant(delta);
    
    this->Lock(this);
    
    std::vector<ScriptComponent*>::iterator i = m_components.begin();
    for (i; i != m_components.end(); i++) {
        Entity* parent = (*i)->GetParent();
        Variant* p = new Variant(parent);
        if(parent) {
            (*i)->SetGlobal("GameObject", p);
        }
        
        (*i)->CallFunction("Update", 1, &d);
        
        delete p;
    }
    
    delete d;
    this->Unlock();
}

void ScriptingSystem::RegisterEventHandler(std::string type, std::string funcName) {
    // Get current thread out of isolate
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    ScriptThread* thread = (ScriptThread*)isolate->GetData(0);
    ScriptComponent* component = thread->GetCurrentScript();
    
    // Get the registered type ID
    MessageSystem* messageSystem = GetSystem<MessageSystem>();
    uint32_t typeID = messageSystem->GetMessageType(type);
    
    EventHandler* eh = new EventHandler;
    eh->type = typeID;
    eh->funcName = funcName;
    eh->component = component;
    
    m_eventHandlers.push_back(eh);
    
    // Register with message system
    messageSystem->RegisterCallback(this, type, ScriptEventHandler);
}

void ScriptingSystem::ScriptEventHandler(GigaObject* obj, Message* message) {
    Variant* mv = new Variant(message);
    
    ScriptingSystem* scriptingSystem = (ScriptingSystem*)obj;
    scriptingSystem->Lock(scriptingSystem);
    
    std::vector<EventHandler*>::iterator it = scriptingSystem->m_eventHandlers.begin();
    for(; it != scriptingSystem->m_eventHandlers.end(); it++) {
        if((*it)->type == message->type) {
            Entity* ent = (*it)->component->GetParent();
            Variant* parent = 0;
            if(ent) {
                parent = new Variant(ent);
                (*it)->component->SetGlobal("GameObject", parent);
            }
            
            (*it)->component->CallFunction((*it)->funcName, 1, &mv);
            
            if(parent) {
                delete parent;
            }
        }
    }
    
    scriptingSystem->Unlock();
    delete mv;
}
