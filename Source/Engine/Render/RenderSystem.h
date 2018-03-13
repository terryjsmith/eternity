
#ifndef rendersystem_h
#define rendersystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Render/Scene.h>
#include <Render/RenderPass.h>

class GIGA_API RenderSystem : public System {
public:
	RenderSystem() = default;
	virtual ~RenderSystem() = default;

	/**
	 * Separate initialize to setup framebuffers
	 */
	void Setup(int windowWidth, int windowHeight);

	/**
	 * Default create functions
	 */
	virtual VertexBuffer* CreateVertexBuffer();
	virtual VertexType* CreateVertexType();

	/**
	 * Draw things on the screen
	 */
	virtual void Render(Scene* scene) { }

protected:
	std::vector<RenderPass*> m_renderPasses;
};

#endif