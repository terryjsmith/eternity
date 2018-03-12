
#ifndef opengldeferredrenderpass_h
#define opengldeferredrenderpass_h

#include <eternity.h>
#include <Render/RenderPass.h>

class GIGA_API OpenGLDeferredRenderPass : public RenderPass {
public:
	OpenGLDeferredRenderPass() = default;
	~OpenGLDeferredRenderPass() = default;

	void Initialize(int windowWidth, int windowHeight);
	void Render(Scene* scene);

protected:
	void RecursiveRender(MeshComponent* mesh, matrix4 parent);
};

#endif