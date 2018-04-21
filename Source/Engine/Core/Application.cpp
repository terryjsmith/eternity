
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

}

void Application::Startup() {
	// Initialize all systems
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		System* sys = (*i)->system;
		sys->Initialize();
	}
    
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

	// Register singletons
	MetaSystem* metaSystem = GetSystem<MetaSystem>();
	metaSystem->RegisterSingleton("ResourceSystem", resourceSystem);
    metaSystem->RegisterSingleton("InputSystem", inputSystem);
}

void Application::Update(float delta) {
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();

	for (; i != m_systems.end(); i++) {
		if ((*i)->tickRate > 0) {
			(*i)->accumulator += delta;

			if ((*i)->accumulator > (1.0f / (*i)->tickRate)) {
				float theta = (1.0f / (*i)->tickRate);
				(*i)->accumulator -= theta;
				(*i)->system->Update(theta);
			}
		}
	}

	i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		if ((*i)->tickRate == 0) {
			(*i)->system->Update(delta);
		}
	}
}

void Application::Shutdown() {
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		System* sys = (*i)->system;
		sys->Shutdown();
	}
}

Application* Application::GetInstance() {
	if (m_instance == 0) {
		m_instance = new Application();
	}

	return(m_instance);
}
