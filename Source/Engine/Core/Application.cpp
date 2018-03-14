
#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Core/MessageSystem.h>
#include <Render/Shader.h>
#include <Core/Error.h>

Application* Application::m_instance = 0;

void Application::Initialize() {

}

void Application::Startup() {
	// Initialize all systems
	std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
	for (; i != m_systems.end(); i++) {
		System* sys = (*i)->system;
		sys->Initialize();
	}

	// Register messages
	MessageSystem* messageSystem = GetSystem<MessageSystem>();
	Error::MSGTYPE_ERROR = messageSystem->RegisterMessageType<Error>("Error");

	// Register resource types
	ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
	resourceSystem->RegisterResourceType<Shader>("Shader");
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