
#include <IO/ResourceSystem.h>
#include <Core/Error.h>

ResourceObject* ResourceSystem::LoadResource(std::string filename, std::string type) {
	Resource* resource = 0;

	// Check to see if we've loaded this resource
	std::vector<Resource*>::iterator it = m_resources.begin();
	for (; it != m_resources.end(); it++) {
		if ((*it)->filename == filename) {
			resource = *it;
			break;
		}
	}

	// If we have a resource, we probably also have a resource object to return
	std::map<Resource*, ResourceObject*>::iterator ri = m_resourceObjects.find(resource);
	if (ri != m_resourceObjects.end()) {
		return(ri->second);
	}

	// If not, find and initialize the resource
	if (resource == 0) {
		// First, get the full path by searching out search paths
		std::string fullpath = FindResourcePath(filename);
		if (fullpath.empty()) {
			// Error
			Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to locate resource file", filename), true);
			return(0);
		}
		
		Resource* r = new Resource();
		r->Initialize(fullpath, FILEMODE_READ | FILEMODE_BINARY);
		r->type = type;

		m_resources.push_back(r);
		resource = r;
	}

	// Create a new ResourceObject of the necessary type
	ResourceObject* obj = 0;
	for (size_t i = 0; i < m_resourceTypes.size(); i++) {
		if (m_resourceTypes[i]->name == type) {
			obj = m_resourceTypes[i]->createFunc();
			break;
		}
	}

	GIGA_ASSERT(obj != 0, "Resource type not found.");

	// Create an instance of the Resource into the ResourceObject
	obj->Instantiate(resource);
	m_resourceObjects[resource] = obj;

	return(obj);
}

void ResourceSystem::AddSearchPath(std::string path) {
	m_paths.push_back(path);
}

std::string ResourceSystem::FindResourcePath(std::string filename) {
	std::string ret;

	// First try to open the file with the path it's on now
	if (Resource::Exists(filename)) {
		return(filename);
	}

	// Loop through our list of files, try to open this one
	std::vector<std::string>::iterator i = m_paths.begin();
	for (; i != m_paths.end(); i++) {
		std::string fn(*i);
		fn += "/";
		fn += filename;

		if (Resource::Exists(fn)) {
			return(fn);
		}
	}

	return(ret);
}
