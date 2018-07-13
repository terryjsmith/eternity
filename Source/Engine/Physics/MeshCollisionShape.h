
#ifndef meshcollisionshape_h
#define meshcollisionshape_h

#include <eternity.h>
#include <Physics/CollisionShape.h>
#include <Render/Mesh.h>

GIGA_CLASS()
class GIGA_API MeshCollisionShape : public CollisionShape {
public:
    MeshCollisionShape();
    ~MeshCollisionShape();
    
    GIGA_CLASS_NAME("MeshCollisionShape");
    GIGA_CLASS_BODY();
    
    /**
     * Initialize from a set of vertices and indices 
     */
    void Initialize(Mesh* mesh);
    
    /**
     * After deserialization
     */
    void PostDeserialize();
    
protected:
    // Our triangle mesh array (index and vertex buffer) if this is a static object
    btTriangleIndexVertexArray* m_triMesh;
    
    // Our original mesh
    GIGA_VARIABLE(Serialize) Mesh* m_mesh;
};

#endif
