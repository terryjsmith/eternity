
#ifndef terraincomponent_hpp
#define terraincomponent_hpp

#include <eternity.h>
#include <Core/Component.h>
#include <Render/TerrainQuad.h>

class GIGA_API TerrainComponent : public Component {
public:
    TerrainComponent();
    ~TerrainComponent() = default;
    
    GIGA_CLASS_NAME("TerrainComponent");
    
    /**
     * Load or unload streaming textures
     */
    void MaybeLoad(BoundingBox* aabb);
    
    /**
     * Serialization
     */
    void Deserialize(DataRecord* record);
    void Serialize(DataRecord* record);
    
    /**
     * Get quads
     */
    TerrainQuad* GetQuad(int index);
    
    /**
     * Add texture
     */
    bool AddTexture(Texture2D* tex, int slot = 0);
    
public:
    // The splat map for applying texture
    Texture2D* splat;
    
    // Textures applied
    Texture2D* textures[4];
    
    // Additional (optional) textures
    Texture2D* normalTexture;
    
protected:
    // Terrain broken down into a quadtree
    TerrainQuad* m_quads[4];
    
    // Heightmap
    Texture2D* m_heightmap;
    
    // Area
    float m_width;
    float m_startX, m_startZ;
};

#endif
