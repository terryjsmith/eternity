
#ifndef renderpass_h
#define renderpass_h

#include <eternity.h>
#include <Render/Scene.h>
#include <Render/ShaderProgram.h>
#include <Render/Framebuffer.h>

class GIGA_API RenderPass {
public:
	RenderPass();
    ~RenderPass();

    virtual void Initialize(int windowWidth, int windowHeight);
	virtual void Render(Scene* scene) = 0;

    /**
     * Get a framebuffer
     */
    Framebuffer* GetFramebuffer(int index);
    
protected:
    // Framebuffers attached to this render pass
    std::vector<Framebuffer*> m_framebuffers;
    
	ShaderProgram* m_program;
	int m_windowWidth, m_windowHeight;
};

#endif
