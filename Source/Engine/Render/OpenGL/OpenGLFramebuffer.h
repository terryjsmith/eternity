
#ifndef opengframebuffer_h
#define opengframebuffer_h

#include <eternity.h>
#include <Render/Framebuffer.h>

/**
 * Single framebuffer object to which multiple render targets can be attached
 */
class GIGA_API OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer();
    virtual ~OpenGLFramebuffer();
    
    /**
     * Initialize (will detroy any existing framebuffer and textures)
     */
    void Initialize();
    
    /**
     * Attach a texture to our framebuffer
     */
    void AddTexture(Texture2D* texture, int type);
    void AddTexture(Texture3D* texture, int type, int slot);
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    void SetTexture(Texture2D* texture, int type);
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    void SetTexture(Texture3D* texture, int type, int slot);
    
    /**
     * Use this framebuffer for rendering/output
     */
    void Bind();
    void Unbind();
    
protected:
    // Our internal framebuffer ID
    unsigned int m_framebuffer;
};

#endif
