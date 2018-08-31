
#ifndef terrainquad_hpp
#define terrainquad_hpp

#include <eternity.h>
#include <Render/MeshComponent.h>
#include <Render/Texture2D.h>

#define TERRAIN_QUAD_SIZE       64
#define TERRAIN_SCALE_FACTOR    0.25

class GIGA_API TerrainQuad : public MeshComponent {
public:
    TerrainQuad();
    ~TerrainQuad();
    
    /**
     * Set the area this quad represents
     */
    void Initialize(float startX, float startZ, float width, Texture2D* heightmap, Texture2D* splat);
    
    /**
     * Load or unload streaming textures
     */
    void MaybeLoad(BoundingBox* aabb);
    void Unload();
    
    /**
     * Set textures
     */
    void SetTexture(int slot, Texture2D* texture);
    
    /**
     * Whether this is loaded or not
     */
    bool IsLoaded() { return m_loaded; }
    
public:
    // The splat map for applying texture
    Texture2D* splat;
    
    // Textures applied
    Texture2D* textures[4];
    
protected:
    // The hieghtmap we will load from
    Texture2D* m_heightmap;
    
    // Whether the heightmap is loaded or not
    bool m_loaded;
    
    // Area
    float m_width;
    float m_startX, m_startZ;
    
    // Our bounding box
    BoundingBox m_aabb;
};

#endif
