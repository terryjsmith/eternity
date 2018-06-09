
#ifndef rendersystem_h
#define rendersystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Render/Scene.h>
#include <Render/RenderPass.h>
#include <Render/Texture2D.h>
#include <Render/VertexBuffer.h>
#include <Render/IndexBuffer.h>
#include <Render/Framebuffer.h>

GIGA_CLASS()
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
    virtual IndexBuffer* CreateIndexBuffer();
	virtual VertexType* CreateVertexType();
    virtual Texture2D* CreateTexture2D();
    virtual Texture3D* CreateTexture3D();
    virtual Framebuffer* CreateFramebuffer();
    virtual ShaderProgram* CreateShaderProgram();

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
	void SetCurrentScene(Scene* scene) { m_currentScene = scene; }
	GIGA_FUNCTION() Scene* GetCurrentScene() { return m_currentScene; }

protected:
    // Post processing passes
	std::vector<RenderPass*> m_renderPasses;
    
    // Window width and height
    int m_windowWidth, m_windowHeight;
    
    // Current scene
	Scene* m_currentScene;
};

#endif
