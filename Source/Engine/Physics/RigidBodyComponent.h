
#ifndef physicscomponent_h
#define physicscomponent_h

#include <eternity.h>
#include <Core/Component.h>
#include <Core/MathTypes.h>
#include <Core/Transform.h>
#include <Physics/CollisionShape.h>

GIGA_CLASS()
class GIGA_API RigidBodyComponent : public btMotionState, public Component {
public:
    RigidBodyComponent();
    ~RigidBodyComponent();
    
    GIGA_CLASS_NAME("RigidBodyComponent");
    GIGA_CLASS_BODY();
    
    GIGA_FUNCTION() Transform* GetTransform() { return m_transform; }
    
    /**
     * Initialize (after shape is attached)
     */
    void Initialize(CollisionShape* shape, float mass);
    
    /**
     * Get/set mass
     */
    GIGA_FUNCTION() float GetMass() { return m_mass; }
    GIGA_FUNCTION() void SetMass(float mass);
    
    /**
     * Get/set velocity
     */
    GIGA_FUNCTION() vector3 GetVelocity() { return m_velocity; }
    GIGA_FUNCTION() void SetVelocity(vector3 velocity);
    
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
    void OnComponentAdded(Component* component);
    
    /**
     * After deserialization
     */
    void PostDeserialize();
    
protected:
    // Override functions
    void getWorldTransform(btTransform &worldTrans) const;
    void setWorldTransform(const btTransform &worldTrans);
    
protected:
    // Collision shape
    GIGA_VARIABLE(Serialize) CollisionShape* m_shape;
    
    // Our Bullet world rigid body
    btRigidBody* m_rigidBody;
    
    // Current velocity
    GIGA_VARIABLE(Serialize) vector3 m_velocity;
    
    // The mass of our object
    GIGA_VARIABLE(Serialize) float m_mass;
    
    // Our transform (generally also the parent transform for other components)
    GIGA_VARIABLE(Serialize) Transform* m_transform;
  
    // Offset from centre to the rest of the world
    vector3 m_offsetPosition;
    
private:
    // Local copy of position and rotation
    vector3 m_position;
    quaternion m_rotation;
};

#endif
