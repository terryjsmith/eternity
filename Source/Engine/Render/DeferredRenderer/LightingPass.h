
#ifndef lightingpass_hpp
#define lightingpass_hpp

#include <eternity.h>
#include <Render/RenderPass.h>

class GIGA_API LightingPass : public RenderPass {
public:
    LightingPass();
    ~LightingPass() = default;
    
    void Initialize(int windowWidth, int windowHeight);
    void Render(Scene* scene);
    
    /**
     * Set textures
     */
    void SetDiffuseTexture(Texture2D* diffuseTexture) { m_diffuseTexture = diffuseTexture; }
    void SetNormalTexture(Texture2D* normalTexture) { m_normalTexture = normalTexture; }
    void SetPositionTexture(Texture2D* positionTexture) { m_positionTexture = positionTexture; }
    void SetMaterialTexture(Texture2D* materialTexture) { m_materialTexture = materialTexture; }
    
protected:
    // Ortho matrix for screen/framebuffer render
    matrix4 m_ortho;
    
    // Vertex buffer
    VertexBuffer* m_buffer;
    
    // Links to other buffers
    Texture2D* m_diffuseTexture;
    Texture2D* m_normalTexture;
    Texture2D* m_positionTexture;
    Texture2D* m_materialTexture;
    
    Texture3D* m_null;
};

#endif
