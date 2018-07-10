
#ifndef resourceobject_h
#define resourceobject_h

#include <eternity.h>
#include <IO/Resource.h>
#include <Core/GigaObject.h>

class ResourceSystem;

class GIGA_API ResourceObject : public GigaObject {
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
    
    /**
     * After deserialization
     */
    void PostDeserialize();

protected:
	/**
	* Initialize with a resource
	*/
	void Instantiate(Resource* resource);

	friend class ResourceSystem;

protected:
	GIGA_VARIABLE(Serialize) Resource* m_resource;
};

#endif
