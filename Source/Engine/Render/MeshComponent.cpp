
#include <Render/MeshComponent.h>
#include <Core/Entity.h>
#include <Render/RenderSystem.h>
#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Core/DataRecord.h>

void MeshComponent::Instantiate(Mesh* mesh) {
	// Save base mesh
	m_mesh = mesh;

	// Remove previous children
	std::vector<MeshComponent*>::iterator ci = m_children.begin();
	for (; ci != m_children.end(); ci++) {
		delete *ci;
	}
	m_children.clear();

	// Instatiate children
    std::vector<Mesh*> children = mesh->children;
	std::vector<Mesh*>::iterator it = children.begin();
	for (; it != children.end(); it++) {
		MeshComponent* child = new MeshComponent();
		child->Instantiate(*it);

		m_children.push_back(child);
	}
}

void MeshComponent::Deserialize(DataRecord* record) {
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    Mesh* mesh = (Mesh*)resourceSystem->LoadResource(record->Get("mesh")->AsString(), "Mesh");
    
    m_transform->SetLocalPosition(record->Get("position")->AsVector3());
    m_transform->SetLocalScaling(record->Get("scale")->AsVector3());
    m_transform->SetLocalRotation(record->Get("rotation")->AsQuaternion());
    
    if(mesh != m_mesh) {
        this->Instantiate(mesh);
        m_mesh = mesh;
    }
}

void MeshComponent::Serialize(DataRecord* record) {
    record->Set("mesh", new Variant(m_mesh->GetResource()->filename));
    record->Set("position", new Variant(m_transform->GetLocalPosition()));
    record->Set("rotation", new Variant(m_transform->GetLocalRotation()));
    record->Set("scale", new Variant(m_transform->GetLocalScaling()));
    record->Set("entityID", new Variant(m_parent->ID()));
}
