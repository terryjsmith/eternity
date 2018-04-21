
#ifndef rendersystem_h
#define rendersystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Render/Scene.h>
#include <Render/RenderPass.h>

class GIGA_API RenderSystem : public System {
public:
	RenderSystem() : m_currentScene(0) { }
	virtual ~RenderSystem() = default;

	GIGA_CLASS_NAME("RenderSystem");

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
	virtual void Render() { }

	/**
	 * Get/set current scene
	 */
	void SetCurrentScene(Scene* scene) { m_currentScene = scene; }
	Scene* GetCurrentScene() { return m_currentScene; }

protected:
	std::vector<RenderPass*> m_renderPasses;
	Scene* m_currentScene;
};

#endif