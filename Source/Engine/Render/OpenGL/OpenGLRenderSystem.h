
#ifndef openglrendersystem_h
#define openglrendersystem_h

#include <eternity.h>
#include <Render/RenderSystem.h>
#include <Render/DeferredRenderer/GBuffer.h>
#include <Render/DeferredRenderer/CombinePass.h>
#include <Render/DeferredRenderer/LightingPass.h>
#include <Render/DeferredRenderer/TerrainPass.h>

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
    Texture3D* CreateTexture3D();
    Framebuffer* CreateFramebuffer();
    ShaderProgram* CreateShaderProgram();

	/**
	* Draw things on the screen
	*/
	void Render();
    
    /**
     * Clear buffers
     */
    void SetClearColor(vector4 color);
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
     * Blending
     */
    void EnableBlending();
    void DisableBlending();
    void SetBlendFunc(int sourceFactor, int destFactor);
    
    /**
     * Set viewport (normally done by framebuffer)
     */
    void SetViewport(int width, int height);
    
    /**
     * Set render type (front/back/none)
     */
    void SetDrawBuffer(int buffer);

	/**
	 * Use default framebuffer
	 */
	void UseDefaultFramebuffer();

	/**
	 * References to textures
	 */
	Texture2D* GetPositionTexture();
	Texture2D* GetNormalTexture();
	Texture2D* GetLightingTexture();
	Texture2D* GetDiffuseTexture();
	Texture2D* GetIndexTexture();

protected:
	// Default render passes
	GBuffer* m_gbufferRenderPass;
    CombinePass* m_combineRenderPass;
    LightingPass* m_lightingRenderPass;
    TerrainPass* m_terrainRenderPass;
};

#endif
