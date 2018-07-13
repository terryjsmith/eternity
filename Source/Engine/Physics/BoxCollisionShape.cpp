
#include <Physics/BoxCollisionShape.h>

void BoxCollisionShape::Initialize(vector3 extents, vector3 offset) {
    // Calculate the half extents of the bounding box
    vector3 halfExtents = extents / 2.0f;
    
    // Create our collision shape
    m_collisionShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
    m_offset = offset + halfExtents;
}

void BoxCollisionShape::PostDeserialize() {
    this->Initialize(m_extents, m_offset);
}
