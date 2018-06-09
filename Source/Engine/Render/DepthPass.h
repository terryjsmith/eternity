
#ifndef depthpass_h
#define depthpass_h

#include <eternity.h>
#include <Render/Texture2D.h>
#include <Render/CameraComponent.h>
#include <Render/Scene.h>
#include <Render/Framebuffer.h>
#include <Render/ShaderProgram.h>

class GIGA_API DepthPass {
public:
    DepthPass() = default;
    ~DepthPass() = default;
    
    /**
     * Initialize
     */
    void Initialize(int width, int height);
    
    /**
     * Look at
     */
    void SetView(CameraComponent* camera) { m_camera = camera; }
    
    /**
     * Render
     */
    void Render(Scene* scene);
    
    /**
     * Get/set texture
     */
    Texture2D* GetTexture() { return m_depthTexture; }
    
    void SetTexture(Texture2D* tex);
    void SetTexture(Texture3D* tex, int slot);
    
protected:
    void RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent);
    
protected:
    // Camera component
    CameraComponent* m_camera;
    
    // Temporary depth map texture for each face
    Texture2D* m_depthTexture;
    
    // Framebuffer for doing depth pass
    Framebuffer* m_framebuffer;
    
    // Depth shader
    ShaderProgram* m_program;
};

#endif
