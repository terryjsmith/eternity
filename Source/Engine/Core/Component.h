
#ifndef component_h
#define component_h

#include <eternity.h>
#include <Core/GigaObject.h>

class Entity;

class GIGA_API Component : public GigaObject {
public:
	Component() = default;
	virtual ~Component() = default;

protected:
	// What type of component are we?
	uint32_t m_typeID;

	// Parent entity
	Entity* m_parent;
};

#endif
