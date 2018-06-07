
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

public:
    // Child mesh objects
	std::vector<Mesh*> children;

    // Vertex buffer
	VertexBuffer *vertexBuffer;
    
    // Index buffer
    IndexBuffer* indexBuffer;
    
    // Diffuse texture
    Texture2D* diffuseTexture;
    
    // Additional (optional) textures
    Texture2D* normalTexture;
    
    // Material
    Material* material;
    
    // Number of triangles (must be set for rendering)
    int numTriangles;
    
    // Bounding box of vertices (min and max)
    BoundingBox aabb;
    
    // Bones
    std::vector<Bone*> bones;
    
    // Skeletal animations
    std::vector<Animation*> animations;
};

#endif
