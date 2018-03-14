
#ifndef mesh_h
#define mesh_h

#include <eternity.h>
#include <Render/VertexBuffer.h>
#include <IO/ResourceObject.h>

class GIGA_API Mesh : public ResourceObject {
public:
	Mesh() = default;
	~Mesh();

	GIGA_CLASS_NAME("Mesh");

public:
	std::vector<Mesh*> children;

	VertexBuffer *vertexBuffer;
};

#endif