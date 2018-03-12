
#ifndef resourceobject_h
#define resourceobject_h

#include <eternity.h>
#include <IO/Resource.h>

class ResourceSystem;

class GIGA_API ResourceObject {
public:
	ResourceObject() = default;
	virtual ~ResourceObject();

	/**
	 * Process loaded data
	 */
	virtual void ProcessData() { }

	/**
	 * Get resource
	 */
	Resource* GetResource();

protected:
	/**
	* Initialize with a resource
	*/
	void Instantiate(std::shared_ptr<Resource>& resource);

	friend class ResourceSystem;

protected:
	std::shared_ptr<Resource> m_resource;
};

#endif