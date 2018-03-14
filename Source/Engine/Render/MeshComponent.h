
#ifndef meshcomponent_h
#define meshcomponent_h

#include <eternity.h>
#include <Render/Mesh.h>
#include <Render/RenderComponent.h>

GIGA_CLASS(Scriptable)
class GIGA_API MeshComponent : public RenderComponent {
public:
	MeshComponent() = default;
	~MeshComponent() = default;

	GIGA_CLASS_NAME("MeshComponent");

	GIGA_FUNCTION(Scriptable) void Instantiate(Mesh* mesh);

	std::vector<MeshComponent*>& GetChildren() { return m_children; }
	Mesh* GetMesh() { return m_mesh; }

protected:
	std::vector<MeshComponent*> m_children;

	GIGA_VARIABLE(Scriptable, Serializable) Mesh* m_mesh;
};

#endif