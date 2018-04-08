
#include <Core/MetaSystem.h>

void MetaSystem::RegisterFunction(std::string className, std::string functionName, CallableFunction c) {
	std::string key = className + "::" + functionName;
	m_functions[key] = c;
}

CallableFunction MetaSystem::FindFunction(std::string className, std::string functionName) {
	std::string key = className + "::" + functionName;
	std::map<std::string, CallableFunction>::iterator it = m_functions.find(key);
	if (it != m_functions.end()) {
		return(it->second);
	}

	return(0);
}

GigaObject* MetaSystem::CreateObject(std::string className) {
	std::map<std::string, GigaObject* (*)()>::iterator it = m_constructors.find(className);
	if (it != m_constructors.end()) {
		return(it->second());
	}

	return(0);
}

void MetaSystem::RegisterSingleton(std::string className, GigaObject* instance) {
	m_singletons[className] = instance;
}

GigaObject* MetaSystem::GetSingleton(std::string className) {
	std::map<std::string, GigaObject*>::iterator it = m_singletons.find(className);
	if (it != m_singletons.end()) {
		return(it->second);
	}

	return(0);
}