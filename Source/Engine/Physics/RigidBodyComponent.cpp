
#include <Physics/RigidBodyComponent.h>
#include <Core/Entity.h>
#include <Core/TransformComponent.h>

RigidBodyComponent::RigidBodyComponent() {
    m_collisionShape = 0;
    m_rigidBody = 0;
    m_velocity = vector3(0);
    m_mass = 0;
    m_offsetPosition = vector3(0);
    
    m_transform = new Transform();
    m_transform->SetRoot(true);
}

RigidBodyComponent::~RigidBodyComponent() {
    if(m_transform) {
        delete m_transform;
        m_transform = 0;
    }
    
    if(m_collisionShape) {
        delete m_collisionShape;
        m_collisionShape = 0;
    }
    
    if(m_rigidBody) {
        delete m_rigidBody;
        m_rigidBody = 0;
    }
}

void RigidBodyComponent::OnEntityAssigned() {
    // Get other components, find a transform (if applicable)
    TransformComponent* tc = m_parent->FindComponent<TransformComponent>();
    if(tc) {
        m_transform->SetParent(tc->GetTransform());
    }
}

void RigidBodyComponent::UpdateTransform() {
    m_position = GetTransform()->GetWorldPosition();
    m_rotation = GetTransform()->GetWorldRotation();
}

void RigidBodyComponent::getWorldTransform(btTransform &worldTrans) const {
    btTransform initialTransform;
    initialTransform.setIdentity();
    
    vector3 position = m_position;
    //position += m_offsetPosition;
    
    initialTransform.setOrigin(btVector3(position.x, position.y, position.z));
    initialTransform.setRotation(btQuaternion(m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w));
    
    worldTrans = initialTransform;
}

void RigidBodyComponent::setWorldTransform(const btTransform &worldTrans) {
    // Get the rotation and positions from our new world transform
    btQuaternion rotation = worldTrans.getRotation();
    btVector3 position = worldTrans.getOrigin();
    
    GetTransform()->SetWorldPosition(vector3(position.x(), position.y(), position.z()) - m_offsetPosition);
    GetTransform()->SetWorldRotation(quaternion(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
}
