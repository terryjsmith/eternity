
#include <Render/Framebuffer.h>

Framebuffer::Framebuffer() {
    m_complete = false;
}

Framebuffer::~Framebuffer() {
    if (m_textures.size()) {
        for (int i = 0; i < m_textures.size(); i++) {
            delete m_textures[i];
        }
        
        m_textures.clear();
    }
}
