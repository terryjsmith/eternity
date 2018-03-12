
#ifndef openglrendersystem_h
#define openglrendersystem_h

#include <eternity.h>
#include <Render/RenderSystem.h>
#include <Render/OpenGL/OpenGLDeferredRenderPass.h>

class GIGA_API OpenGLRenderSystem : public RenderSystem {
public:
	OpenGLRenderSystem() = default;
	~OpenGLRenderSystem() = default;

	void Initialize();

	/**
	 * Create things
	 */
	VertexBuffer* CreateVertexBuffer();
	VertexType* CreateVertexType();

	/**
	* Draw things on the screen
	*/
	void Render(Scene* scene);

protected:
	// Default render passes
	OpenGLDeferredRenderPass* m_deferredRenderPass;
};

#endif