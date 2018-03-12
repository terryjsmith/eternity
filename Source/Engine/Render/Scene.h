
#ifndef scene_h
#define scene_h

#include <eternity.h>
#include <Render/CameraComponent.h>
#include <Render/MeshComponent.h>

class GIGA_API Scene {
public:
	Scene() = default;
	~Scene() = default;

	void AddMesh(MeshComponent* mesh);
	std::vector<MeshComponent*>& GetMeshes() { return m_meshes; }

	void SetCamera(CameraComponent* camera) { m_camera = camera; }
	CameraComponent* GetCamera() { return m_camera; }

protected:
	std::vector<MeshComponent*> m_meshes;
	CameraComponent* m_camera;
};

#endif