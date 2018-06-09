
#include <Render/OpenGL/OpenGL.h>
#include <Render/OpenGL/OpenGLFramebuffer.h>
#include <Core/Error.h>

int FRAMEBUFFER_SLOT_0 = GL_COLOR_ATTACHMENT0;
int FRAMEBUFFER_SLOT_1 = GL_COLOR_ATTACHMENT1;
int FRAMEBUFFER_SLOT_2 = GL_COLOR_ATTACHMENT2;
int FRAMEBUFFER_SLOT_3 = GL_COLOR_ATTACHMENT3;
int FRAMEBUFFER_SLOT_4 = GL_COLOR_ATTACHMENT4;
int FRAMEBUFFER_SLOT_5 = GL_COLOR_ATTACHMENT5;
int FRAMEBUFFER_SLOT_DEPTH = GL_DEPTH_ATTACHMENT;

OpenGLFramebuffer::OpenGLFramebuffer() {
    m_framebuffer = 0;
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
    if (m_framebuffer) {
        GL_CHECK(glDeleteFramebuffers(1, &m_framebuffer));
        m_framebuffer = 0;
    }
}

void OpenGLFramebuffer::Initialize(int width, int height) {
    m_width = width;
    m_height = height;
    
    if (m_framebuffer) {
        GL_CHECK(glDeleteFramebuffers(1, &m_framebuffer));
    }
    
    if (m_textures.size()) {
        for (int i = 0; i < m_textures.size(); i++) {
            delete m_textures[i];
        }
        
        m_textures.clear();
    }
    
    // Initialize our framebuffer
    GL_CHECK(glGenFramebuffers(1, &m_framebuffer));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
}

void OpenGLFramebuffer::AddTexture(Texture2D* texture, int type) {
    // Attach the texture to the FBO
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture->GetTexture(), 0));
    
    // Don't add depth textures to our framebuffer slots/counts
    if (type == GL_DEPTH_ATTACHMENT) {
        return;
    }
    
    m_textures.push_back(texture);
    m_slots.push_back(type);
    
    if (m_slots.size()) {
        GL_CHECK(glDrawBuffers(m_slots.size(), &m_slots[0]));
    }
    else {
        GL_CHECK(glDrawBuffers(m_slots.size(), GL_NONE));
    }
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to attach framebuffer."));
        assert(false); // Probably don't want to repeat this over and over again
    }
    
    m_complete = true;
}

void OpenGLFramebuffer::AddTexture(Texture3D* texture, int type, int slot) {
    // Attach the texture to the FBO
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, type, slot, texture->GetTexture(), 0));
    
    // Don't add depth textures to our framebuffer slots/counts
    if (type == GL_DEPTH_ATTACHMENT) {
        return;
    }
    
    m_textures.push_back(texture);
    m_slots.push_back(type);
    
    if (m_slots.size()) {
        GL_CHECK(glDrawBuffers(m_slots.size(), &m_slots[0]));
    }
    else {
        GL_CHECK(glDrawBuffers(m_slots.size(), GL_NONE));
    }
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to attach framebuffer."));
        assert(false); // Probably don't want to repeat this over and over again
    }
    
    m_complete = true;
}

void OpenGLFramebuffer::SetTexture(Texture2D* texture, int type) {
    // Attach the texture to the FBO
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, type, texture->GetTarget(0), texture->GetTexture(), 0));
}

void OpenGLFramebuffer::SetTexture(Texture3D* texture, int type, int slot) {
    // Attach the texture to the FBO
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, type, slot, texture->GetTexture(), 0));
}

void OpenGLFramebuffer::Bind() {
    assert(m_framebuffer > 0);
    
    // Attach the relevant buffers
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
    if (m_slots.size()) {
        GL_CHECK(glDrawBuffers(m_slots.size(), &m_slots[0]));
    }
    else {
        GL_CHECK(glDrawBuffers(m_slots.size(), GL_NONE));
    }
    
    if (m_complete == false) {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to attach framebuffer."));
            assert(false); // Probably don't want to repeat this over and over again
        }
        
        m_complete = true;
    }
    
    GL_CHECK(glViewport(0, 0, m_width, m_height));
}

void OpenGLFramebuffer::Unbind() {
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
