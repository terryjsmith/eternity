
#ifndef metasystem_h
#define metasystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Core/Variant.h>
#include <Core/GigaObject.h>

class GIGA_API MetaSystem : public System {
public:
	MetaSystem() = default;
	~MetaSystem() = default;

	/**
	 * Register a class-specific callback
	 */
	void RegisterFunction(std::string className, std::string functionName, CallableFunction c);

	/**
	 * Find a class-specific callback function
	 */
	CallableFunction FindFunction(std::string className, std::string functionName);

	/**
	 * Create a new object of type className
	 */
	GigaObject* CreateObject(std::string className);

	/**
	 * Register a new class type
	 */
	template<class T> void RegisterClassName(std::string className) {
		m_constructors[className] = &CreateGigaObject<T>;
	}

	/**
	 * Register a singleton instance
	 */
	void RegisterSingleton(std::string className, GigaObject* instance);

	/**
	 * Get singleton instance
	 */
	GigaObject* GetSingleton(std::string className);

protected:
	// Create a new C++ object based on a registered script interface type
	template<typename T> static GigaObject* CreateGigaObject() { return new T; }

protected:
	// Callable meta functions
	std::map<std::string, CallableFunction> m_functions;

	// Callable constructors
	std::map<std::string, GigaObject* (*)()> m_constructors;

	// Registered singletons
	std::map<std::string, GigaObject*> m_singletons;
};

#endif