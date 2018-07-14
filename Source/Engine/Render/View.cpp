
#include <Render/View.h>

void View::AddMesh(MeshComponent* mesh) {
	m_meshes.push_back(mesh);
}

void View::AddLight(LightComponent* light) {
    m_lights.push_back(light);
}
