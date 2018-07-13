
#include <Physics/CollisionShape.h>

CollisionShape::CollisionShape() {
    m_collisionShape = 0;
    m_offset = vector3(0);
}

CollisionShape::~CollisionShape() {
    if(m_collisionShape) {
        if(m_collisionShape) {
            delete m_collisionShape;
            m_collisionShape = 0;
        }
    }
}
