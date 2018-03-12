
#include <Render/MeshComponent.h>

void MeshComponent::Instantiate(Mesh* mesh) {
	// Save base mesh
	m_mesh = mesh;

	// Instatiate children
	std::vector<Mesh*>::iterator it = mesh->children.begin();
	for (; it != mesh->children.end(); it++) {
		MeshComponent* child = new MeshComponent();
		child->Instantiate(*it);

		m_children.push_back(child);
	}
}