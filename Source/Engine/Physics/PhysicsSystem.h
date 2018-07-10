
#ifndef physicssystem_h
#define physicssystem_h

#include <eternity.h>
#include <Core/ComponentSystem.h>
#include <Physics/RigidBodyComponent.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

GIGA_CLASS()
class GIGA_API PhysicsSystem : public ComponentSystem<RigidBodyComponent> {
public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;
    
    /**
     * Initialize our physics library
     */
    void Initialize();
    
    /**
     * Update our physics (step simulation)
     */
    void Update(float delta);
    
    /**
     * Add a physics body to our active physics world
     */
    void AddRigidBody(RigidBodyComponent* rigidBody);
    
    GIGA_CLASS_NAME("PhysicsSystem");
    
protected:
    // Physics collision tracking
    struct TrackedCollision {
        RigidBodyComponent* objA;
        RigidBodyComponent* objB;
        bool active;
    };
    
protected:
    // A list of active collisions
    std::vector<TrackedCollision> m_collisions;
    
    // Bullet stuff
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btBroadphaseInterface* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
};

#endif
