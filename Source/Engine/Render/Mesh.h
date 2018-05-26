
#ifndef mesh_h
#define mesh_h

#include <eternity.h>
#include <IO/ResourceObject.h>
#include <Render/VertexBuffer.h>
#include <Render/IndexBuffer.h>
#include <Render/Texture2D.h>
#include <Render/BoundingBox.h>
#include <Render/Bone.h>
#include <Render/Material.h>
#include <Render/Animation.h>

class GIGA_API Mesh : public ResourceObject {
public:
	Mesh() = default;
	~Mesh();

	GIGA_CLASS_NAME("Mesh");
    
    /**
     * Load a mesh from a .g3d file
     */
    void ProcessData();
    
    /**
     * Mesh file format sections
     */
    enum {
        SECTION_MESH = 1,
        SECTION_BONES,
        SECTION_ANIMATIONS,
        SECTION_LAST
    };

protected:
    // Child mesh objects
	std::vector<Mesh*> m_children;

    // Vertex buffer
	VertexBuffer *m_vertexBuffer;
    
    // Index buffer
    IndexBuffer* m_indexBuffer;
    
    // Diffuse texture
    Texture2D* m_diffuseTexture;
    
    // Additional (optional) textures
    Texture2D* m_normalTexture;
    
    // Material
    Material* m_material;
    
    // Number of triangles (must be set for rendering)
    int m_numTriangles;
    
    // Bounding box of vertices (min and max)
    BoundingBox m_aabb;
    
    // Bones
    std::vector<Bone*> m_bones;
    
    // Skeletal animations
    std::vector<Animation*> m_animations;
};

#endif
