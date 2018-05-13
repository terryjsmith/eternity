
#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Core/MessageSystem.h>
#include <Core/MetaSystem.h>
#include <IO/InputSystem.h>
#include <Render/Shader.h>
#include <Core/Error.h>
#include <IO/Keyboard.h>
#include <IO/Command.h>

Application* Application::m_instance = 0;

Application::Application() {
	m_primaryWindow = 0;
}

void Application::Initialize() {
	// Get systems
    InputSystem* inputSystem = GetSystem<InputSystem>();

	// Register messages
	MessageSystem* messageSystem = GetSystem<MessageSystem>();
	messageSystem->RegisterMessageType<Error>("Error");
	messageSystem->RegisterMessageType<KeyboardMessage>("KeyboardMessage");
	messageSystem->RegisterMessageType<Command>("Command");

	// Register resource types
	ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
	resourceSystem->RegisterResourceType<Shader>("Shader");

	// Register singletons and callback functions 
	MetaSystem* metaSystem = GetSystem<MetaSystem>();
	metaSystem->RegisterMetaFunctions();

	metaSystem->RegisterSingleton("ResourceSystem", resourceSystem);
	metaSystem->RegisterSingleton("InputSystem", inputSystem);
}

void Application::Shutdown() {
	
}

Application* Application::GetInstance() {
	if (m_instance == 0) {
		m_instance = new Application();
	}

	return(m_instance);
}

World* Application::GetWorld() {
	if (m_world == 0) {
		m_world = new World();
	}

	return(m_world);
}