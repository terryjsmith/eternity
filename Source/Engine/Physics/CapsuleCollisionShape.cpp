
#include <Physics/CapsuleCollisionShape.h>

void CapsuleCollisionShape::Initialize(float radius, float height) {
    m_height = height;
    m_radius = radius;
    
    // Create a new capsule shape
    m_collisionShape = new btCapsuleShape(radius, height);
}

void CapsuleCollisionShape::PostDeserialize() {
    Initialize(m_radius, m_height);
}
