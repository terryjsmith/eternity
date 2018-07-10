
#include <Physics/MeshCollisionComponent.h>
#include <Physics/PhysicsSystem.h>
#include <Core/Application.h>

MeshCollisionComponent::MeshCollisionComponent() {
    m_triMesh = 0;
}

MeshCollisionComponent::~MeshCollisionComponent() {
    if(m_triMesh) {
        delete m_triMesh;
        m_triMesh = 0;
    }
}

void MeshCollisionComponent::Initialize(float* vertices, int vcount, uint32_t* indices, int icount, bool complete) {
    m_mass = 0.0f;
    btIndexedMesh* mesh = new btIndexedMesh();
    
    mesh->m_indexType = PHY_INTEGER;
    mesh->m_numTriangles = icount / 3;
    mesh->m_vertexType = PHY_FLOAT;
    mesh->m_triangleIndexBase = (unsigned char*)indices;
    mesh->m_triangleIndexStride = 3 * sizeof(uint32_t);
    mesh->m_vertexBase = (unsigned char*)vertices;
    mesh->m_vertexStride = 3 * sizeof(float);
    mesh->m_numVertices = vcount;
    
    if(m_triMesh == 0) {
        m_triMesh = new btTriangleIndexVertexArray();
    }
    m_triMesh->addIndexedMesh(*mesh);
    
    if(complete) {
        m_collisionShape = new btBvhTriangleMeshShape(m_triMesh, true);
        
        // Calculate inertia
        btVector3 inertia;
        m_rigidBody = new btRigidBody(0.0f, this, m_collisionShape, inertia);
        m_rigidBody->setUserPointer(this);
        
        // Add to physics world
        PhysicsSystem* physicsSystem = GetSystem<PhysicsSystem>();
        physicsSystem->AddRigidBody(this);
    }
}
