
#include <Core/MetaSystem.h>

void MetaSystem::RegisterFunction(std::string className, std::string functionName, CallableFunction c, bool isStatic) {
	std::map<std::string, RegisteredClass*>::iterator it = m_classes.find(className);
	GIGA_ASSERT(it != m_classes.end(), "Class type not registered.");

	std::map<std::string, RegisteredFunction*>::iterator fi = it->second->functions.find(functionName);
	GIGA_ASSERT(fi == it->second->functions.end(), "Function already registered.");

	RegisteredFunction* fn = new RegisteredFunction();
	fn->name = functionName;
	fn->func = c;
	fn->isStatic = isStatic;

	it->second->functions[functionName] = fn;
}

CallableFunction MetaSystem::FindFunction(std::string className, std::string functionName) {
	std::map<std::string, RegisteredClass*>::iterator it = m_classes.find(className);
	GIGA_ASSERT(it != m_classes.end(), "Class type not registered.");

	std::map<std::string, RegisteredFunction*>::iterator fi = it->second->functions.find(functionName);
	GIGA_ASSERT(fi != it->second->functions.end(), "Function not registered.");

	return(fi->second->func);
}

GigaObject* MetaSystem::CreateObject(std::string className) {
	std::map<std::string, RegisteredClass*>::iterator it = m_classes.find(className);
	GIGA_ASSERT(it != m_classes.end(), "Class type not registered.");

	return(it->second->ctor());
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

std::vector<MetaSystem::RegisteredClass*> MetaSystem::GetRegisteredClasses() {
    std::vector<MetaSystem::RegisteredClass*> classes;
    std::map<std::string, MetaSystem::RegisteredClass*>::iterator it = m_classes.begin();
    for(; it != m_classes.end(); it++) {
        classes.push_back(it->second);
    }
    
    return(classes);
}
