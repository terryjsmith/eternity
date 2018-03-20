
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