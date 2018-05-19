
#include <Render/RenderWindow.h>
#include <Render/Mesh.h>
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Render/Shader.h>
#include <Render/OpenGL/OpenGLShaderProgram.h>
#include <Render/Scene.h>
#include <IO/ResourceSystem.h>
#include <IO/InputSystem.h>
#include <Core/MetaSystem.h>
#include <Core/MessageSystem.h>
#include <IO/Keyboard.h>
#include <Core/Timer.h>
#include <Core/Application.h>
#include <Core/Entity.h>
#include <Scripting/ScriptComponent.cpp>

int main(int argc, char** argv) {
	Application* application = Application::GetInstance();
	World* world = application->GetWorld();

	OpenGLRenderSystem* renderSystem = world->CreateSystem<OpenGLRenderSystem>();
	ResourceSystem* resourceSystem = world->CreateSystem<ResourceSystem>();
	MessageSystem* messageSystem = world->CreateSystem<MessageSystem>(20);
    InputSystem* inputSystem = world->CreateSystem<InputSystem>();
    MetaSystem* metaSystem = world->CreateSystem<MetaSystem>();
    ScriptingSystem* scriptingSystem = world->CreateSystem<ScriptingSystem>(20);

	world->Initialize();
	application->Initialize();

	RenderWindow* window = new RenderWindow();
	window->Create("Test Window", 800, 600, false);

	application->SetPrimaryWindow(window);

	// Specify resource directories
    resourceSystem->AddSearchPath("Resources");
	resourceSystem->AddSearchPath("Resources/Shaders");
    resourceSystem->AddSearchPath("Resources/Scripts");
    
    // Get framebuffer size (retina is different)
    int framebufferWidth, framebufferHeight;
    window->GetFramebufferSize(framebufferWidth, framebufferHeight);
    
	// Send window width and height to render system for setup
	renderSystem->Setup(framebufferWidth, framebufferHeight);

	// Create a scene
	Scene* scene = new Scene();
	renderSystem->SetCurrentScene(scene);

	// Create a camera
    Entity* camera = world->CreateEntity();
    CameraComponent* cameraComponent = camera->Assign<CameraComponent>();
	scene->SetCamera(cameraComponent);

	cameraComponent->GetTransform()->SetWorldPosition(vector3(0, 0, 4));

	// Create a description of our vertex data
	VertexType* type = renderSystem->CreateVertexType();
	type->Create();

	type->AddVertexAttrib(VERTEXTYPE_ATTRIB_POSITION, 3, 0);
	type->AddVertexAttrib(VERTEXTYPE_ATTRIB_COLOR, 3, 3);

	// Create some vertex data
	float data[] = {
		-0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -1.0f, 0.0, 1.0f, 0.0f,
		0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f
	};

	// Create mesh
	Mesh* mesh = new Mesh();
	mesh->vertexBuffer = renderSystem->CreateVertexBuffer();
	mesh->vertexBuffer->Create(type, 3, data, false);

    Entity* triangle = world->CreateEntity();
    MeshComponent* meshComponent = triangle->Assign<MeshComponent>();
	meshComponent->Instantiate(mesh);

	scene->AddMesh(meshComponent);

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

	Timer* gameTimer = new Timer();
	gameTimer->Start();

	float delta = 0.0f;

	while (window->IsClosing() == false) {
		delta = gameTimer->Duration();
		gameTimer->Reset();

		world->Update(delta);

		/*if (moveForward) {
			RenderSystem* renderSystem = GetSystem<RenderSystem>();
			Scene* scene = renderSystem->GetCurrentScene();
			CameraComponent* camera = scene->GetCamera();

			Transform* transform = camera->GetTransform();
			transform->Move(transform->GetLook() * delta);
		}*/

		window->ProcessEvents();

		renderSystem->Render();
		window->SwapBuffer();
	}

	world->Shutdown();
	application->Shutdown();

	delete mesh;
	delete renderSystem;
	delete window;

	return(0);
}
