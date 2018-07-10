
#include <Physics/BoxCollisionComponent.h>
#include <Physics/PhysicsSystem.h>
#include <Core/Application.h>

void BoxCollisionComponent::Initialize(vector3 extents, vector3 position, vector3 offset, float mass) {
    m_mass = mass;
    
    // Calculate the half extents of the bounding box
    vector3 halfExtents = extents / 2.0f;
    
    // Create our collision shape
    m_collisionShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
    m_offsetPosition = offset + halfExtents;
    
    // Set position
    GetTransform()->SetLocalPosition(position + offset + halfExtents);
    UpdateTransform();
    
    // Calculate inertia
    btVector3 inertia;
    m_collisionShape->calculateLocalInertia(mass, inertia);
    
    // Create a new rigid body for our physics world
    m_rigidBody = new btRigidBody(mass, this, m_collisionShape, inertia);
    m_rigidBody->setUserPointer(this);
    
    // Add to physics world
    PhysicsSystem* physicsSystem = GetSystem<PhysicsSystem>();
    physicsSystem->AddRigidBody(this);
}
