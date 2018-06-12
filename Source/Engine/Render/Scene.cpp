
#include <Render/Scene.h>

void Scene::AddMesh(MeshComponent* mesh) {
	m_meshes.push_back(mesh);
}

void Scene::AddLight(LightComponent* light) {
    m_lights.push_back(light);
}
