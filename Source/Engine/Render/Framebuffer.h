
#ifndef framebuffer_h
#define framebuffer_h

extern int FRAMEBUFFER_SLOT_0;
extern int FRAMEBUFFER_SLOT_1;
extern int FRAMEBUFFER_SLOT_2;
extern int FRAMEBUFFER_SLOT_3;
extern int FRAMEBUFFER_SLOT_4;
extern int FRAMEBUFFER_SLOT_5;
extern int FRAMEBUFFER_SLOT_DEPTH;

#include <eternity.h>
#include <Render/Texture2D.h>
#include <Render/Texture3D.h>

/**
 * Single framebuffer object to which multiple render targets can be attached
 */
class GIGA_API Framebuffer {
public:
    Framebuffer();
    virtual ~Framebuffer();
    
    /**
     * Initialize (will detroy any existing framebuffer and textures)
     */
    virtual void Initialize() { }

	/**
	 * Destroy all textures and this framebuffer
	 */
	virtual void Destroy() { }
    
    /**
     * Attach a texture to our framebuffer
     */
    virtual void AddTexture(Texture2D* texture, int type) { }
    virtual void AddTexture(Texture3D* texture, int type, int slot) { }
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    virtual void SetTexture(Texture2D* texture, int type) { }
    
    /**
     * Set a texture to a specific slot (one-time, not permanent)
     */
    virtual void SetTexture(Texture3D* texture, int type, int slot) { }
    
    /**
     * Get a texture back out
     */
    Texture* GetTexture(int index) { return m_textures[index]; }
    
    /**
     * Use this framebuffer for rendering/output
     */
    virtual void Bind() { }
    virtual void Unbind() { }
    
protected:
    // The textures attached to this framebuffer
    std::vector<Texture*> m_textures;
    
    // The color/output slots this framebuffer draws to
    std::vector<unsigned int> m_slots;
    
    // Whether this framebuffer has been checked for completeness
    bool m_complete;
};

#endif
