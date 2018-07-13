
#include <Physics/PhysicsCollision.h>

PhysicsCollision::PhysicsCollision() {
    m_type = 0;
    m_bodyA = m_bodyB = 0;
}

PhysicsCollision::PhysicsCollision(int type, RigidBodyComponent* a, RigidBodyComponent* b) {
    m_type = type;
    m_bodyA = a;
    m_bodyB = b;
}
