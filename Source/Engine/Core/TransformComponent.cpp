
#include <Core/TransformComponent.h>
#include <Core/Entity.h>

TransformComponent::TransformComponent() {
    m_transform = new Transform();
}

TransformComponent::~TransformComponent() {
    if(m_transform) {
        delete m_transform;
        m_transform = 0;
    }
}

void TransformComponent::PostDeserialize() {
    m_parent->AddComponent(this);
}
