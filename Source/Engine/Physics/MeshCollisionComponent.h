
#ifndef meshcollisioncomponent_h
#define meshcollisioncomponent_h

#include <eternity.h>
#include <Physics/RigidBodyComponent.h>

GIGA_CLASS()
class GIGA_API MeshCollisionComponent : public RigidBodyComponent {
public:
    MeshCollisionComponent();
    ~MeshCollisionComponent();
    
    GIGA_CLASS_NAME("MeshCollisionComponent");
    GIGA_CLASS_BODY();
    
    /**
     * Initialize from a set of vertices and indices 
     * (complete flag set to true for final of multiple sub-meshes)
     */
    void Initialize(float* vertices, int vcount, uint32_t* indices, int icount, bool complete);
    
protected:
    // Our triangle mesh array (index and vertex buffer) if this is a static object
    btTriangleIndexVertexArray* m_triMesh;
};

#endif
