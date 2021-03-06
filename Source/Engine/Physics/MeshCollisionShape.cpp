
#include <Physics/MeshCollisionShape.h>

MeshCollisionShape::MeshCollisionShape() {
    m_triMesh = 0;
}

MeshCollisionShape::~MeshCollisionShape() {
    if(m_triMesh) {
        delete m_triMesh;
        m_triMesh = 0;
    }
}

void MeshCollisionShape::Initialize(Mesh* mesh) {
    m_mesh = mesh;
    
    // Instatiate children
    std::vector<Mesh*>::iterator it = mesh->children.begin();
    for (; it != mesh->children.end(); it++) {
        btIndexedMesh* mesh = new btIndexedMesh();
        
        int vcount, icount;
        float* vertices = (*it)->GetVertices(vcount);
        unsigned int* indices = (*it)->GetIndices(icount);
        
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
        
        free(vertices);
        free(indices);
    }
    
    m_collisionShape = new btBvhTriangleMeshShape(m_triMesh, true);
}

void MeshCollisionShape::PostDeserialize() {
    Initialize(m_mesh);
}
