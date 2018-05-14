
#ifndef application_h
#define application_h

#include <eternity.h>
#include <Core/System.h>
#include <Core/Window.h>
#include <Core/World.h>

class GIGA_API Application {
public:
	Application();
	~Application() = default;

	/**
	* Startup (called after systems are created)
	*/
	void Initialize();

	/**
	* Shutdown
	*/
	void Shutdown();

	/**
	 * Get/set primary window
	 */
	void SetPrimaryWindow(Window* window) { m_primaryWindow = window; }
	Window* GetPrimaryWindow() { return m_primaryWindow; }

	/**
	 * Get game world
	 */
	World* GetWorld();

	/**
	* Get singleton instance
	*/
	static Application* GetInstance();

protected:
	// Singleton
	static Application* m_instance;
    
    // Primary render window
	Window* m_primaryWindow;

	// Game world
	World* m_world;
};

// Short-hand class to find sub-systems in the Application class
template<class T> T* GetSystem() {
    World* world = Application::GetInstance()->GetWorld();
    return(world->GetSystem<T>());
}

#endif
