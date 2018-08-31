
#ifndef terrainsystem_hpp
#define terrainsystem_hpp

#include <eternity.h>
#include <Core/ComponentSystem.h>
#include <Render/TerrainComponent.h>
#include <Render/CameraComponent.h>

/**
 * Terrain system responsible for managing and rendering terrain components
 */
class GIGA_API TerrainSystem : public ComponentSystem<TerrainComponent> {
public:
    TerrainSystem() = default;
    ~TerrainSystem() = default;
    
    GIGA_CLASS_NAME("TerrainSystem");
    
    void Initialize();
    void Update(float delta);
    void Shutdown();
    
    /**
     * Set the camera for streaming and LOD
     */
    void SetCamera(CameraComponent* camera) { m_camera = camera; }
    
    /**
     * Enable/disable streaming
     */
    void EnableStreaming(bool streaming) { m_streaming = streaming; }
    
    /**
     * Get/set load distance (defines bounding box for loaded areas of terrain)
     */
    float GetLoadDistance() { return m_loadDistance; }
    void SetLoadDistance(float distance) { m_loadDistance = distance; }
    
    /**
     * Get all terrain components
     */
    std::vector<TerrainComponent*>& GetComponents() { return m_components; }
    
protected:
    CameraComponent* m_camera;
    bool m_streaming;
    float m_loadDistance;
};

#endif
