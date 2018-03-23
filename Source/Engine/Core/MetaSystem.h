
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

protected:
	// Callable meta functions
	std::map<std::string, CallableFunction> m_functions;
};

#endif