
#ifndef physicscomponent_h
#define physicscomponent_h

#include <eternity.h>
#include <Core/Component.h>
#include <Core/MathTypes.h>
#include <Core/Transform.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class GIGA_API RigidBodyComponent : public btMotionState, public Component {
public:
    RigidBodyComponent();
    ~RigidBodyComponent();
    
    GIGA_FUNCTION() Transform* GetTransform() { return m_transform; }
    
    /**
     * Get physics world rigid body object
     */
    btRigidBody* GetRigidBody() { return m_rigidBody; }
    
    /**
     * Update local transform variables
     */
    void UpdateTransform();
    
    /**
     * Overridable notifications
     */
    void OnEntityAssigned();
    
protected:
    // Override functions
    void getWorldTransform(btTransform &worldTrans) const;
    void setWorldTransform(const btTransform &worldTrans);
    
protected:
    // Our Bullet collision shape
    btCollisionShape* m_collisionShape;
    
    // Our Bullet world rigid body
    btRigidBody* m_rigidBody;
    
    // Current velocity
    vector3 m_velocity;
    
    // The mass of our object
    float m_mass;
    
    // Our transform (generally also the parent transform for other components)
    Transform* m_transform;
  
    // Offset from centre to the rest of the world
    vector3 m_offsetPosition;
    
private:
    // Local copy of position and rotation
    vector3 m_position;
    quaternion m_rotation;
};

#endif
