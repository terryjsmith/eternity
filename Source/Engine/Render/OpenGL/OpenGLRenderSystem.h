
#ifndef openglrendersystem_h
#define openglrendersystem_h

#include <eternity.h>
#include <Render/RenderSystem.h>
#include <Render/DeferredRenderer/GBuffer.h>
#include <Render/DeferredRenderer/CombinePass.h>
#include <Render/DeferredRenderer/LightingPass.h>

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
    IndexBuffer* CreateIndexBuffer();
    Texture2D* CreateTexture2D();
    Framebuffer* CreateFramebuffer();
    ShaderProgram* CreateShaderProgram();

	/**
	* Draw things on the screen
	*/
	void Render();
    
    /**
     * Clear buffers
     */
    void Clear(int bitmask);
    
    /**
     * Draw + indexed draw
     */
    void Draw(int type, int elements);
    void DrawIndexed(int type, int elements);
    
    /**
     * Depth testing
     */
    void EnableDepthTest(int type);
    void DisableDepthTest();
    
    /**
     * Set viewport (normally done by framebuffer)
     */
    void SetViewport(int width, int height);
    
    /**
     * Clear any current framebuffer
     */
    void ClearFramebuffer();

protected:
	// Default render passes
	GBuffer* m_gbufferRenderPass;
    CombinePass* m_combineRenderPass;
    LightingPass* m_lightingRenderPass;
};

#endif
