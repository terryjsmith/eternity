
#include <Render/RenderWindow.h>
#include <Render/Mesh.h>
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Render/Shader.h>
#include <Render/OpenGL/OpenGLShaderProgram.h>
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
#include <IO/SQLiteDataLoader.h>
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
    debugLog->Initialize("client.log", FILEMODE_READWRITE);
    
	OpenGLRenderSystem* renderSystem = world->CreateSystem<OpenGLRenderSystem>();
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

	RenderWindow* window = new RenderWindow();
	window->Create("Test Window", 800, 600, false);

	application->SetPrimaryWindow(window);
    replicationSystem->SetType(REPLICATION_CLIENT);

	// Specify resource directories
	resourceSystem->AddSearchPath("");
	resourceSystem->AddSearchPath("Resources/Shaders");
    resourceSystem->AddSearchPath("Resources/Scripts");
    resourceSystem->AddSearchPath("Resources/Models");
    resourceSystem->AddSearchPath("Resources/Textures");
    
    // Get framebuffer size (retina is different)
    int framebufferWidth, framebufferHeight;
    window->GetFramebufferSize(framebufferWidth, framebufferHeight);
    
	// Send window width and height to render system for setup
	renderSystem->Setup(framebufferWidth, framebufferHeight);

	// Create a scene
	View* scene = new View();
	renderSystem->SetCurrentView(scene);
    
    /* Load some data
    SQLiteDataLoader* loader = application->CreateAppService<SQLiteDataLoader>();
    loader->Open("game.db");

	std::map<std::string, std::string> args;
	args["m_sceneID"] = std::to_string(1);
	std::vector<std::string> componentTypes = Component::GetComponentTypes();
	for (size_t i = 0; i < componentTypes.size(); i++) {
		std::vector<DataRecord*> components = loader->GetRecords(componentTypes[i], args);
	}
    
    //loader->SaveRecords();
    
    loader->Close();*/

	// Create a camera
    Entity* camera = world->CreateEntity();
    CameraComponent* cameraComponent = camera->Assign<CameraComponent>();
	scene->SetCamera(cameraComponent);

	cameraComponent->GetTransform()->SetWorldPosition(vector3(0, 1.5, 4));

	/* Create mesh
    Mesh* mesh = dynamic_cast<Mesh*>(resourceSystem->LoadResource("crate.g3d", "Mesh"));

    Entity* crate = world->CreateEntity();
    MeshComponent* meshComponent = crate->Assign<MeshComponent>();
	meshComponent->Instantiate(mesh);
    meshComponent->GetTransform()->SetWorldPosition(vector3(1, 10, 0));

	scene->AddMesh(meshComponent);
    
    // Add physics
    BoxCollisionComponent* boxCollider = crate->Assign<BoxCollisionComponent>();
    boxCollider->Initialize(vector3(1.0f), vector3(0), vector3(-0.5f, 0, -0.5f), 10.0f);*/
    
    
    
    /* Add physics
    MeshCollisionComponent* meshCollider = floor->Assign<MeshCollisionComponent>();
    int vcount, icount;
    float* vertices = floorMesh->children[0]->GetVertices(vcount);
    unsigned int* indices = floorMesh->children[0]->GetIndices(icount);
    meshCollider->Initialize(vertices, vcount, indices, icount, true);*/
    
    // Create a light
    Entity* light = world->CreateEntity();
    /*SpotLightComponent* spotLight = light->Assign<SpotLightComponent>();
    spotLight->Initialize();
    spotLight->GetTransform()->SetWorldPosition(vector3(0, 2, 3));
    //spotLight->GetCamera()->GetTransform()->Rotate(vector3(0, 1, 0), 180.0f);
    spotLight->GetCamera()->GetTransform()->Rotate(vector3(1, 0, 0), -30.0f);
    spotLight->SetFOV(90.0f);
    spotLight->SetAttenuation(25.0f);
    
    scene->AddLight(spotLight);
    
    PointLightComponent* pointLight = light->Assign<PointLightComponent>();
    pointLight->Initialize();
    pointLight->GetTransform()->SetWorldPosition(vector3(-1, 3, 2));
    pointLight->SetAttenuation(20.0f);
    
    scene->AddLight(pointLight);*/

    // Create a keyboard
    Keyboard* keyboard = new Keyboard();
    keyboard->Initialize();
    
    inputSystem->RegisterInputDevice(keyboard);
    
    scriptingSystem->Lock(0);
    
    Script* gamejs = dynamic_cast<Script*>(resourceSystem->LoadResource("game.js", "Script"));
    ScriptComponent* gameComponent = new ScriptComponent();
    
    gameComponent->Initialize(gamejs);
    
    Script* clientjs = dynamic_cast<Script*>(resourceSystem->LoadResource("client.js", "Script"));
    ScriptComponent* clientComponent = camera->Assign<ScriptComponent>();
    
    clientComponent->Initialize(clientjs);
    
    scriptingSystem->Unlock();
    
    // Set ambient lighting
    scene->SetAmbientLight(vector3(0.25, 0.25, 0.25));

	Timer* gameTimer = new Timer();
	gameTimer->Start();

	float delta = 0.0f;

	while (window->IsClosing() == false) {
		delta = gameTimer->Duration();
		gameTimer->Reset();

		world->Update(delta);

		window->ProcessEvents();

		renderSystem->Render();
		window->SwapBuffer();
	}

	world->Shutdown();
	application->Shutdown();

	delete renderSystem;
	delete window;

	return(0);
}
