
#include <Render/Mesh.h>

Mesh::~Mesh() {
	if (vertexBuffer) {
		delete(vertexBuffer);
		vertexBuffer = 0;
	}
}