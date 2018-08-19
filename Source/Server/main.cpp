
#include <Render/RenderWindow.h>
#include <Render/Mesh.h>
#include <Render/Shader.h>
#include <Render/View.h>
#include <Render/MaterialSystem.h>
#include <IO/ResourceSystem.h>
#include <IO/InputSystem.h>
#include <Core/MetaSystem.h>
#include <Core/MessageSystem.h>
#include <IO/Keyboard.h>
#include <Core/Timer.h>
#include <Core/Application.h>
#include <Core/Entity.h>
#include <Scripting/ScriptingSystem.h>
#include <Render/PointLightComponent.h>
#include <Render/SpotLightComponent.h>
#include <Physics/PhysicsSystem.h>
#include <Physics/RigidBodyComponent.h>
#include <Render/RenderSystem.h>
#include <IO/SQLiteDataLoader.h>
#include <Core/TimeSystem.h>
#include <Network/NetworkSystem.h>
#include <Network/ReplicationSystem.h>
#include <Core/TimeSystem.h>
#include <Core/Error.h>

Resource* debugLog;

void WriteDebug(GigaObject* obj, Message* msg) {
    std::string finalout;
    finalout.resize(8000);
    
    Error* err = (Error*)msg;
    
    tm t;
    mktime(&t);
    
    std::string levelstr;
    switch(err->GetErrorLevel()) {
        case Error::ERROR_INFO:
            levelstr = "INFO";
            break;
        case Error::ERROR_DEBUG:
            levelstr = "DEBUG";
            break;
        case Error::ERROR_WARN:
            levelstr = "WARN";
            break;
        case Error::ERROR_FATAL:
            levelstr = "FATAL";
    };
    
    std::string datepart;
    datepart.resize(100);
    
    strftime((char*)datepart.data(), 100, "%m/%d/%Y %H:%M", &t);
    
    std::string line = err->GetErrorMsg();
    int length = sprintf((char*)finalout.data(), "[%s %-5s]: %s", datepart.c_str(), levelstr.c_str(), line.c_str());
    finalout.resize(length);
    
    debugLog->WriteLine(finalout);
}

int main(int argc, char** argv) {
    Application* application = Application::GetInstance();
    World* world = application->GetWorld();
    
    // Open debug log
    debugLog = new Resource();
    debugLog->Initialize("server.log", FILEMODE_READWRITE);
    
    RenderSystem* renderSystem = world->CreateSystem<RenderSystem>();
    MaterialSystem* materialSystem = world->CreateSystem<MaterialSystem>();
    ResourceSystem* resourceSystem = world->CreateSystem<ResourceSystem>();
    MessageSystem* messageSystem = world->CreateSystem<MessageSystem>(20);
    InputSystem* inputSystem = world->CreateSystem<InputSystem>();
    MetaSystem* metaSystem = world->CreateSystem<MetaSystem>();
    ScriptingSystem* scriptingSystem = world->CreateSystem<ScriptingSystem>(20);
    PhysicsSystem* physicsSystem = world->CreateSystem<PhysicsSystem>(20);
    NetworkSystem* networkSystem = world->CreateSystem<NetworkSystem>();
    ReplicationSystem* replicationSystem = world->CreateSystem<ReplicationSystem>(20);
    
    world->Initialize();
    application->Initialize();
    
    messageSystem->RegisterCallback(0, "Error", WriteDebug);
    
    // Specify resource directories
    resourceSystem->AddSearchPath("");
    resourceSystem->AddSearchPath("Resources/Shaders");
    resourceSystem->AddSearchPath("Resources/Scripts");
    resourceSystem->AddSearchPath("Resources/Models");
    resourceSystem->AddSearchPath("Resources/Textures");
    
    // Set replication system
    replicationSystem->SetType(REPLICATION_SERVER);
    networkSystem->Listen(8900);
    
    // Create a scene
    View* scene = new View();
    renderSystem->SetCurrentView(scene);
    
    // Load some data
    SQLiteDataLoader* loader = application->CreateAppService<SQLiteDataLoader>();
    loader->Open("game.db");
    
    std::map<std::string, std::string> args;
    args["sceneID"] = std::to_string(1);
    std::vector<DataRecord*> entities = loader->GetRecords("Entity", args);
    
    for(size_t i = 0; i < entities.size(); i++) {
        Entity* entity = new Entity();
        entity->Deserialize(entities[i]);
        
        world->AddEntity(entity);
        delete entities[i];
    }
    
    entities.clear();
    
    std::vector<std::string> componentTypes = Component::GetComponentTypes();
    for (size_t i = 0; i < componentTypes.size(); i++) {
        DataRecordType* drt = DataRecordType::GetType(componentTypes[i]);
        if(drt == 0)
            continue;
        
        std::vector<DataRecord*> components = loader->GetRecords(componentTypes[i], args);
        for(size_t j = 0; j < components.size(); j++) {
            Component* c = Component::CreateComponent(components[j]->GetType()->GetTypeName());
            c->Deserialize(components[j]);
            
            Entity* entity = world->FindEntity(components[j]->Get("entityID")->AsInt());
            entity->AddComponent(c);
            
            delete components[j];
        }
        
        components.clear();
    }
    
    loader->Close();
    
    scriptingSystem->Lock(0);
    
    Script* gamejs = dynamic_cast<Script*>(resourceSystem->LoadResource("game.js", "Script"));
    ScriptComponent* gameComponent = new ScriptComponent();
    
    gameComponent->Initialize(gamejs);
    
    Script* clientjs = dynamic_cast<Script*>(resourceSystem->LoadResource("server.js", "Script"));
    
    scriptingSystem->Unlock();
    
    // Set ambient lighting
    scene->SetAmbientLight(vector3(0.25, 0.25, 0.25));
    
    Timer* gameTimer = new Timer();
    gameTimer->Start();
    
    float delta = 0.0f;
    
    while (true) {
        delta = gameTimer->Duration();
        gameTimer->Reset();
        
        world->Update(delta);
        
        TimeSystem::Sleep(1);
    }
    
    world->Shutdown();
    application->Shutdown();
    
    return(0);
}
