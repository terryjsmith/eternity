
#ifndef rendersystem_h
#define rendersystem_h

#include <eternity.h>
#include <Core/ComponentSystem.h>
#include <Render/View.h>
#include <Render/RenderPass.h>
#include <Render/Texture2D.h>
#include <Render/VertexBuffer.h>
#include <Render/IndexBuffer.h>
#include <Render/Framebuffer.h>

GIGA_CLASS()
class GIGA_API RenderSystem : public ComponentSystem<RenderComponent> {
public:
	RenderSystem() : m_currentView(0), m_framebuffer(0) { }
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
    virtual IndexBuffer* CreateIndexBuffer();
	virtual VertexType* CreateVertexType();
    virtual Texture2D* CreateTexture2D();
    virtual Texture3D* CreateTexture3D();
    virtual Framebuffer* CreateFramebuffer();
    virtual ShaderProgram* CreateShaderProgram();

	/**
	 * Update scene graph and stuff
	 */
	void Update(float delta);

	/**
	 * Draw things on the screen
	 */
	virtual void Render() { }
    
    /**
     * Clear buffers
     */
    virtual void SetClearColor(vector4 color) { }
    virtual void Clear(int bitmask) { }
    
    /**
     * Draw + indexed draw
     */
    virtual void Draw(int type, int elements) { }
    virtual void DrawIndexed(int type, int elements) { }
    
    /**
     * Depth testing
     */
    virtual void EnableDepthTest(int type) { }
    virtual void DisableDepthTest() { }
    
    /**
     * Blending
     */
    virtual void EnableBlending() { }
    virtual void DisableBlending() { }
    virtual void SetBlendFunc(int sourceFactor, int destFactor) { }
    
    /**
     * Set viewport (normally done by framebuffer)
     */
    virtual void SetViewport(int width, int height) { }
    
    /**
     * Set render type (front/back/none)
     */
    virtual void SetDrawBuffer(int buffer) { }

	/**
	 * Get/set current scene
	 */
	void SetCurrentView(View* scene) { m_currentView = scene; }
	GIGA_FUNCTION() View* GetCurrentView() { return m_currentView; }

	/**
	 * Set/get default framebuffer
	 */
	void SetDefaultFramebuffer(int framebuffer) { m_framebuffer = framebuffer; }
	virtual void UseDefaultFramebuffer() { }

	/**
	 * References to textures
	 */
	virtual Texture2D* GetPositionTexture() { return(0); }
	virtual Texture2D* GetNormalTexture() { return(0); }
	virtual Texture2D* GetLightingTexture() { return(0); }
	virtual Texture2D* GetDiffuseTexture() { return(0); }
	virtual Texture2D* GetIndexTexture() { return(0); }

protected:
    // Post processing passes
	std::vector<RenderPass*> m_renderPasses;
    
    // Window width and height
    int m_windowWidth, m_windowHeight;
    
    // Current scene
	View* m_currentView;

	// default framebuffer
	int m_framebuffer;
};

#endif
