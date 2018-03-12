
#ifndef mesh_h
#define mesh_h

#include <eternity.h>
#include <Render/VertexBuffer.h>

class GIGA_API Mesh {
public:
	Mesh() = default;
	~Mesh();

public:
	std::vector<Mesh*> children;

	VertexBuffer *vertexBuffer;
};

#endif