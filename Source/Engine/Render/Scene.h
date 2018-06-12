
#ifndef scene_h
#define scene_h

#include <eternity.h>
#include <Render/CameraComponent.h>
#include <Render/MeshComponent.h>
#include <Render/LightComponent.h>

/**
 * A scene contains all of the visible objects in the player's area
 */
GIGA_CLASS()
class GIGA_API Scene : public GigaObject {
public:
	Scene() = default;
	~Scene() = default;

	GIGA_CLASS_NAME("Scene");

	GIGA_FUNCTION() void AddMesh(MeshComponent* mesh);
	std::vector<MeshComponent*>& GetMeshes() { return m_meshes; }
    
    GIGA_FUNCTION() void AddLight(LightComponent* light);
    std::vector<LightComponent*>& GetLights() { return m_lights; }

	GIGA_FUNCTION() void SetCamera(CameraComponent* camera) { m_camera = camera; }
	GIGA_FUNCTION() CameraComponent* GetCamera() { return m_camera; }
    
    GIGA_FUNCTION() void SetAmbientLight(vector3 ambient) { m_ambient = ambient; }
    GIGA_FUNCTION() vector3 GetAmbientLight() { return m_ambient; }

protected:
    // The meshes visible to the player
	std::vector<MeshComponent*> m_meshes;
    
    // Lights
    std::vector<LightComponent*> m_lights;
    
    // The active camera component
	CameraComponent* m_camera;
    
    // Ambient lighting
    vector3 m_ambient;
};

#endif
