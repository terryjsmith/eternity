
#include <Core/TransformComponent.h>
#include <Core/Entity.h>
#include <Core/DataRecord.h>

TransformComponent::TransformComponent() {
    m_transform = new Transform();
}

TransformComponent::~TransformComponent() {
    if(m_transform) {
        delete m_transform;
        m_transform = 0;
    }
}

void TransformComponent::Deserialize(DataRecord* record) {
    m_transform->SetLocalPosition(record->Get("position")->AsVector3());
    m_transform->SetLocalScaling(record->Get("scale")->AsVector3());
    m_transform->SetLocalRotation(record->Get("rotation")->AsQuaternion());
}

void TransformComponent::Serialize(DataRecord* record) {
    record->Set("position", new Variant(m_transform->GetLocalPosition()));
    record->Set("rotation", new Variant(m_transform->GetLocalRotation()));
    record->Set("scale", new Variant(m_transform->GetLocalScaling()));
}
