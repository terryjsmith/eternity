
#include <Physics/PhysicsSystem.h>
#include <Physics/PhysicsCollision.h>

void PhysicsSystem::Initialize() {
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_solver,m_collisionConfiguration);
    
    m_dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
}

void PhysicsSystem::Update(float delta) {
    // Update any physics objects
    std::vector<RigidBodyComponent*>::iterator rbi = m_components.begin();
    for(; rbi != m_components.end(); rbi++) {
        (*rbi)->UpdateTransform();
    }

    m_dynamicsWorld->stepSimulation(delta, 6);
    
    // Loop over all current collisions and set them to false
    std::vector<TrackedCollision>::iterator ci = m_collisions.begin();
    for(; ci != m_collisions.end(); ci++) {
        (*ci).active = false;
    }
    
    // Loop over contact manifolds to determine which objects are currently colliding/overlapping
    int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0; i < numManifolds; i++){
        btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* obA = (btCollisionObject*)contactManifold->getBody0();
        btCollisionObject* obB = (btCollisionObject*)contactManifold->getBody1();
        
        btRigidBody* rbA = (btRigidBody*)btRigidBody::upcast(obA);
        btRigidBody* rbB = (btRigidBody*)btRigidBody::upcast(obB);
        
        RigidBodyComponent* bodyA = (RigidBodyComponent*)rbA->getUserPointer();
        RigidBodyComponent* bodyB = (RigidBodyComponent*)rbB->getUserPointer();
        
        // Loop over our current collisions and see if we have this one
        std::vector<TrackedCollision>::iterator it = m_collisions.begin();
        for(; it != m_collisions.end(); i++) {
            if((((*it).objA == bodyA && (*it).objB == bodyB)) || (((*it).objA == bodyB && (*it).objB == bodyA))) {
                (*it).active = true;
                break;
            }
        }
        
        if(it == m_collisions.end()) {
            TrackedCollision newCollision;
            newCollision.objA = bodyA;
            newCollision.objB = bodyB;
            newCollision.active = true;
            
            m_collisions.push_back(newCollision);
            Message::Broadcast(new PhysicsCollision(PhysicsCollision::COLLISION_START, bodyA, bodyB), false);
        }
    }
    
    // Loop over all current collisions that are inactive and send exit events
    for(size_t i = m_collisions.size() - 1; i == 0; i--) {
        TrackedCollision* c = &m_collisions[i];
        if(c->active == true) continue;
        
        Message::Broadcast(new PhysicsCollision(PhysicsCollision::COLLISION_END, c->objA, c->objB), false);
        
        // Clean up
        m_collisions.erase(m_collisions.begin() + i);
    }
    
    // m_dynamicsWorld->debugDrawWorld();
}

void PhysicsSystem::AddRigidBody(RigidBodyComponent* rigidBody) {
    m_dynamicsWorld->addRigidBody(rigidBody->GetRigidBody());
}
