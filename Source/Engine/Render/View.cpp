
#include <Render/View.h>

View::View() {
	m_ambient = vector3(1, 1, 1);
}

void View::AddMesh(MeshComponent* mesh) {
	std::vector<MeshComponent*>::iterator it = std::find(m_meshes.begin(), m_meshes.end(), mesh);
	if (it != m_meshes.end()) {
		return;
	}

	m_meshes.push_back(mesh);
}

void View::AddLight(LightComponent* light) {
	std::vector<LightComponent*>::iterator it = std::find(m_lights.begin(), m_lights.end(), light);
	if (it != m_lights.end()) {
		return;
	}

    m_lights.push_back(light);
}

void View::Clear() {
	m_meshes.clear();
	m_lights.clear();
}