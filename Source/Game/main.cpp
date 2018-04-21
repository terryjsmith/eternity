
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
#include <Core/Application.h>

int main(int argc, char** argv) {
	Application* application = Application::GetInstance();
	application->Initialize();

	OpenGLRenderSystem* renderSystem = application->CreateSystem<OpenGLRenderSystem>();
	ResourceSystem* resourceSystem = application->CreateSystem<ResourceSystem>();
	MessageSystem* messageSystem = application->CreateSystem<MessageSystem>();
    InputSystem* inputSystem = application->CreateSystem<InputSystem>();
    MetaSystem* metaSystem = application->CreateSystem<MetaSystem>();

	application->Startup();

	RenderWindow* window = new RenderWindow();
	window->Create("Test Window", 800, 600, false);

	// Specify resource directories
	resourceSystem->AddSearchPath("Resources/Shaders");

    // Get framebuffer size (retina is different)
    int framebufferWidth, framebufferHeight;
    window->GetFramebufferSize(framebufferWidth, framebufferHeight);
    
	// Send window width and height to render system for setup
	renderSystem->Setup(framebufferWidth, framebufferHeight);

	// Create a scene
	Scene* scene = new Scene();

	// Create a camera
	CameraComponent* camera = new CameraComponent();
	scene->SetCamera(camera);

	camera->GetTransform()->SetWorldPosition(vector3(0, 0, 4));

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

	MeshComponent* meshComponent = new MeshComponent();
	meshComponent->Instantiate(mesh);

	scene->AddMesh(meshComponent);
    
    // Create a keyboard
    Keyboard* keyboard = new Keyboard();
    keyboard->Initialize();
    
    inputSystem->RegisterInputDevice(keyboard);

	while (window->IsClosing() == false) {
		window->ProcessEvents();

		renderSystem->Render(scene);
		window->SwapBuffer();
	}

	delete mesh;
	delete renderSystem;
	delete window;

	return(0);
}
