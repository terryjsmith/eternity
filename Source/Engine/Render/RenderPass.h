
#ifndef renderpass_h
#define renderpass_h

#include <eternity.h>
#include <Render/Scene.h>
#include <Render/ShaderProgram.h>

class GIGA_API RenderPass {
public:
	RenderPass() = default;
	~RenderPass() = default;

	virtual void Initialize(int windowWidth, int windowHeight) { }
	virtual void Render(Scene* scene) = 0;

protected:
	ShaderProgram* m_program;
	int m_windowWidth, m_windowHeight;
};

#endif