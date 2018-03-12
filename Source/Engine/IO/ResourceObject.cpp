
#include <IO/ResourceObject.h>

ResourceObject::~ResourceObject() {
	m_resource.reset();
}

void ResourceObject::Instantiate(std::shared_ptr<Resource>& resource) {
	m_resource = std::shared_ptr<Resource>(resource);

	// Load
	m_resource->Load();

	// Call data processor
	this->ProcessData();

	// Unload
	m_resource->Unload();
}

Resource* ResourceObject::GetResource() {
	return(m_resource.get());
}