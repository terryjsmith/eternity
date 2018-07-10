
#ifndef physicscollision_h
#define physicscollision_h

#include <eternity.h>
#include <Core/Message.h>
#include <Physics/RigidBodyComponent.h>

GIGA_CLASS()
class GIGA_API PhysicsCollision : public Message {
public:
    PhysicsCollision() = default;
    PhysicsCollision(int type, RigidBodyComponent* a, RigidBodyComponent* b) : m_type(type), m_bodyA(a), m_bodyB(b) { }
    ~PhysicsCollision() = default;
    
    enum {
        COLLISION_START = 1,
        COLLISION_END
    };
    
    GIGA_CLASS_NAME("PhysicsCollision");
    
    GIGA_FUNCTION() int GetType() { return m_type; }
    GIGA_FUNCTION() RigidBodyComponent* GetRigidBodyA() { return m_bodyA; }
    GIGA_FUNCTION() RigidBodyComponent* GetRigidBodyB() { return m_bodyB; }
    
protected:
    // Type: start or end?
    int m_type;
    
    // Rigid bodies involved
    RigidBodyComponent* m_bodyA;
    RigidBodyComponent* m_bodyB;
};

#endif
