
#include <Render/MeshComponent.h>
#include <Core/Entity.h>
#include <Render/RenderSystem.h>
#include <Render/Scene.h>
#include <Core/Application.h>

void MeshComponent::Instantiate(Mesh* mesh) {
	// Save base mesh
	m_mesh = mesh;

	// Instatiate children
    std::vector<Mesh*> children = mesh->children;
	std::vector<Mesh*>::iterator it = children.begin();
	for (; it != children.end(); it++) {
		MeshComponent* child = new MeshComponent();
		child->Instantiate(*it);

		m_children.push_back(child);
	}
}

void MeshComponent::PostDeserialize() {
    // Connect back to parent Entity
    m_parent->AddComponent(this);
    
    // Initialize
    this->Instantiate(m_mesh);
}
