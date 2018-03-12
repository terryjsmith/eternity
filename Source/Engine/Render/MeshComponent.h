
#ifndef meshcomponent_h
#define meshcomponent_h

#include <eternity.h>
#include <Render/Mesh.h>
#include <Render/RenderComponent.h>

class GIGA_API MeshComponent : public RenderComponent {
public:
	MeshComponent() = default;
	~MeshComponent() = default;

	void Instantiate(Mesh* mesh);
	std::vector<MeshComponent*>& GetChildren() { return m_children; }
	Mesh* GetMesh() { return m_mesh; }

protected:
	std::vector<MeshComponent*> m_children;
	Mesh* m_mesh;
};

#endif