
#ifndef scriptcomponent_hpp
#define scriptcomponent_hpp

#include <eternity.h>
#include <Scripting/Script.h>
#include <Core/Component.h>
#include <v8.h>

class GIGA_API ScriptComponent : public Component {
    ScriptComponent();
    ~ScriptComponent() = default;
    
    void Initialize(Script* script);
    
protected:
    // Our script
    Script* m_scriptSource;
    
    // Our internal script object
    v8::Persistent<v8::Script, v8::CopyablePersistentTraits<v8::Script>> m_script;
    
    // Our V8 context
    v8::Persistent<v8::Context, v8::CopyablePersistentTraits<v8::Context>> m_context;
};

#endif
