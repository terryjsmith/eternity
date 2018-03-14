
#ifndef shader_h
#define shader_h

#include <eternity.h>
#include <IO/ResourceObject.h>

class GIGA_API Shader : public ResourceObject {
public:
	Shader() = default;
	~Shader() = default;

	GIGA_CLASS_NAME("Shader");
};

#endif