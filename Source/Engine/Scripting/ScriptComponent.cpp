
#include <Scripting/ScriptComponent.h>

ScriptComponent::ScriptComponent() {
    m_scriptSource = 0;
}

void ScriptComponent::Initialize(Script* script) {
    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(m_isolate);
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(m_isolate);
    
    Lock(this);
    m_isolate->SetData(0, this);
    
    // Get our systems
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    // Get all meta class objects
    std::vector<MetaSystem::RegisteredClass*> classes = metaSystem->GetRegisteredClasses();
    std::vector<MetaSystem::RegisteredClass*>::iterator it = classes.begin();
    
    v8::Local<v8::ObjectTemplate> globalSpace;
    v8::Local<v8::Context> context;
    
    // Create a global object template
    globalSpace = v8::ObjectTemplate::New(m_isolate);
    
    // Get our context
    context = v8::Context::New(m_isolate, NULL, globalSpace);
    m_context.Reset(m_isolate, context);
    
    context->Enter();

}
