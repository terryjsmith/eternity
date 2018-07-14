
#ifndef combinepass_hpp
#define combinepass_hpp

#include <eternity.h>
#include <Render/RenderPass.h>

class GIGA_API CombinePass : public RenderPass {
public:
    CombinePass();
    ~CombinePass() = default;
    
    void Initialize(int windowWidth, int windowHeight);
    void Render(View* scene);
    
    /**
     * Set textures
     */
    void SetDiffuseTexture(Texture2D* diffuseTexture) { m_diffuseTexture = diffuseTexture; }
    void SetNormalTexture(Texture2D* normalTexture) { m_normalTexture = normalTexture; }
    void SetPositionTexture(Texture2D* positionTexture) { m_positionTexture = positionTexture; }
    void SetLightingTexture(Texture2D* lightingTexture) { m_lightingTexture = lightingTexture; }
    
    /**
     * Set output framebuffer
     */
    void SetOutputFramebuffer(Framebuffer* framebuffer) { m_framebuffer = framebuffer; }
    
protected:
    // Ortho matrix for screen/framebuffer render
    matrix4 m_ortho;
    
    // Vertex buffer
    VertexBuffer* m_buffer;
    
    // Output framebuffer
    Framebuffer* m_framebuffer;
    
    // Links to other buffers
    Texture2D* m_diffuseTexture;
    Texture2D* m_normalTexture;
    Texture2D* m_positionTexture;
    Texture2D* m_lightingTexture;
};

#endif
