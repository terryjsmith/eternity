
#ifndef script_h
#define script_h

#include <eternity.h>
#include <IO/ResourceObject.h>

GIGA_CLASS()
class GIGA_API Script : public ResourceObject {
public:
	Script() = default;
	~Script() = default;

	GIGA_CLASS_NAME("Script");
};

#endif