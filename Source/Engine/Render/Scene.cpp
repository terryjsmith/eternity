
#include <Render/Scene.h>

void Scene::AddMesh(MeshComponent* mesh) {
	m_meshes.push_back(mesh);
}