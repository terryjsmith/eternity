
#ifndef resourcesystem_h
#define resourcesystem_h

#include <eternity.h>
#include <Core/System.h>
#include <IO/ResourceObject.h>

class GIGA_API ResourceSystem : public System {
public:
	ResourceSystem() = default;
	~ResourceSystem() = default;

	// Internal function to call a C++ constructor
	typedef ResourceObject* (*ResourceObjectCreateFunc)();

	struct ResourceObjectType {
		std::string name;
		ResourceObjectCreateFunc createFunc;
	};

	/**
	* Load a resource
	*/
	ResourceObject* LoadResource(std::string filename, std::string type);

	/**
	* Add a directory/path to search to find files
	*/
	void AddSearchPath(std::string path);

	/**
	* Iterate over search paths to find full path by filename
	*/
	std::string FindResourcePath(std::string filename);

	/**
	* Register a resource type to a class
	*/
	template<class T>
	void RegisterResourceType(std::string className) {
		// If this type is already registered, remove it and override with this new type
		if (m_resourceTypes.size() > 0) {
			for (int i = (int)m_resourceTypes.size() - 1; i >= 0; i--) {
				if (m_resourceTypes[i]->name == className) {
					delete (m_resourceTypes[i]);
					m_resourceTypes.erase(m_resourceTypes.begin() + i);
				}
			}
		}

		ResourceObjectType* type = new ResourceObjectType();
		type->name = className;
		type->createFunc = &CreateResourceObject<T>;
		m_resourceTypes.push_back(type);
	}

protected:
	// Create a new C++ object based on a registered script interface type
	template<typename T> static ResourceObject* CreateResourceObject() { return new T; }

protected:
	// A list of searchable paths
	std::vector<std::string> m_paths;

	// A list of registered resources
	std::vector<std::shared_ptr<Resource>> m_resources;

	// A map of class names to create functions
	std::vector<ResourceObjectType*> m_resourceTypes;
};

#endif