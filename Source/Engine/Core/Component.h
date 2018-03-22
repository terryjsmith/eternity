
#ifndef component_h
#define component_h

#include <eternity.h>
#include <Core/GigaObject.h>

class GIGA_API Component : public GigaObject {
public:
	Component() = default;
	virtual ~Component() = default;

protected:
	uint32_t m_typeID;
};

#endif
