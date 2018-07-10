
#include <Render/RenderComponent.h>
#include <Core/Entity.h>
#include <Core/TransformComponent.h>

RenderComponent::RenderComponent() {
	m_transform = new Transform();
}

RenderComponent::~RenderComponent() {
    if(m_transform) {
        delete m_transform;
        m_transform = 0;
    }
}

void RenderComponent::OnEntityAssigned() {
    // Get other components, find a transform (if applicable)
    TransformComponent* tc = m_parent->FindComponent<TransformComponent>();
    if(tc) {
        m_transform->SetParent(tc->GetTransform());
    }
}

void RenderComponent::OnComponentAdded(Component* component) {
    TransformComponent* tc = dynamic_cast<TransformComponent*>(component);
    if(tc) {
        m_transform->SetParent(tc->GetTransform());
    }
}
