
#ifndef application_h
#define application_h

#include <eternity.h>
#include <Core/System.h>
#include <Render/RenderWindow.h>

class GIGA_API Application {
public:
	Application() = default;
	~Application() = default;

	struct RegisteredSystem {
		int tickRate;
		float accumulator;
		System* system;
	};

	/**
	 * Initialize application default settings (called before systems are created)
	 */
	void Initialize();

	/**
	* Startup (called after systems are created)
	*/
	void Startup();

	/**
	* Shutdown
	*/
	void Shutdown();

	/**
	* Register a new subsystem
	*/
	template<class T>
	T* CreateSystem(int tickRate = 0) {
		T* obj = new T();
		System* sys = dynamic_cast<System*>(obj);
		GIGA_ASSERT(sys != 0, "Class must be inherited from System type.");

		RegisteredSystem* rs = new RegisteredSystem();
		rs->tickRate = tickRate;
		rs->system = sys;
		rs->accumulator = 0;
		m_systems.push_back(rs);

		return(obj);
	}

	/**
	* Find a specific subsystem by class type
	*/
	template<class T>
	T* GetSystem() {
		std::vector<RegisteredSystem*>::iterator i = m_systems.begin();
		for (; i != m_systems.end(); i++) {
			T* object = dynamic_cast<T*>((*i)->system);
			if (object) {
				return(object);
			}
		}

		return(0);
	}

	/**
	* Get singleton instance
	*/
	static Application* GetInstance();

protected:
	// List of registered sub-systems
	std::vector<RegisteredSystem*> m_systems;

	// Singleton
	static Application* m_instance;
    
    // Primary render window
    RenderWindow
};

// Short-hand class to find sub-systems in the Application class
template<class T> T* GetSystem() {
	Application* application = Application::GetInstance();
	return(application->GetSystem<T>());
}

#endif
