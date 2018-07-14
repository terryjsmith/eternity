
#ifndef application_h
#define application_h

#include <eternity.h>
#include <Core/System.h>
#include <Core/Window.h>
#include <Core/World.h>
#include <Core/Service.h>

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
     * Find a service
     */
    template<class T>
    T* GetAppService() {
        std::vector<Service*>::iterator it = m_services.begin();
        for(; it != m_services.end(); it++) {
            T* obj = dynamic_cast<T*>(*it);
            if(obj) {
                return(obj);
            }
        }
        
        return(0);
    }

	/**
	 * Create a new locatable service
	 */
	template<class T>
	T* CreateAppService() {
		T* obj = new T();
		m_services.push_back(obj);

		return(obj);
	}

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
    
    // Running services
    std::vector<Service*> m_services;
};

// Short-hand class to find sub-systems in the Application class
template<class T> T* GetSystem() {
    World* world = Application::GetInstance()->GetWorld();
    return(world->GetSystem<T>());
}

template<class T> T* GetAppService() {
    return(Application::GetInstance()->GetAppService<T>());
}

#endif
