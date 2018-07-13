
#include <Physics/RigidBodyComponent.h>
#include <Core/Entity.h>
#include <Core/TransformComponent.h>
#include <Physics/PhysicsSystem.h>
#include <Core/Application.h>

RigidBodyComponent::RigidBodyComponent() {
    m_rigidBody = 0;
    m_velocity = vector3(0);
    m_mass = 0;
    m_offsetPosition = vector3(0);
    m_shape = 0;
    
    m_transform = new Transform();
    m_transform->SetRoot(true);
}

RigidBodyComponent::~RigidBodyComponent() {
    if(m_transform) {
        delete m_transform;
        m_transform = 0;
    }
    
    if(m_rigidBody) {
        delete m_rigidBody;
        m_rigidBody = 0;
    }
}

void RigidBodyComponent::Initialize(CollisionShape* shape, float mass) {
    m_shape = shape;
    m_mass = mass;
    
    // Calculate inertia
    btCollisionShape* collisionShape = m_shape->GetShape();
    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);
    
    UpdateTransform();
    GetTransform()->SetLocalPosition(m_shape->GetOffset());
    
    // Create a new rigid body for our physics world
    m_rigidBody = new btRigidBody(mass, this, collisionShape, inertia);
    m_rigidBody->setUserPointer(this);
    
    // Add to physics world
    PhysicsSystem* physicsSystem = GetSystem<PhysicsSystem>();
    physicsSystem->AddRigidBody(this);
}

void RigidBodyComponent::SetMass(float mass) {
    
}

void RigidBodyComponent::SetVelocity(vector3 velocity) {
    
}

void RigidBodyComponent::OnEntityAssigned() {
    // Get other components, find a transform (if applicable)
    TransformComponent* tc = m_parent->FindComponent<TransformComponent>();
    if(tc) {
        m_transform->SetParent(tc->GetTransform());
    }
}

void RigidBodyComponent::OnComponentAdded(Component* component) {
    // Get other components, find a transform (if applicable)
    TransformComponent* tc = dynamic_cast<TransformComponent*>(component);
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

void RigidBodyComponent::PostDeserialize() {
    // Connect back to parent Entity
    m_parent->AddComponent(this);
    
    this->Initialize(m_shape, m_mass);
}
