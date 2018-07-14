
#include <Core/Scene.h>

void Scene::AddEntity(Entity* entity) {
	m_entities.push_back(entity);
}

std::vector<Entity*> Scene::GetEntities() {
	return(m_entities);
}