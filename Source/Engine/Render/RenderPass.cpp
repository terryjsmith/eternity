
#include <Render/RenderPass.h>

RenderPass::~RenderPass() {
    for (size_t i = 0; i < m_framebuffers.size(); i++) {
        delete m_framebuffers[i];
    }
    
    m_windowWidth = m_windowHeight = 0;
}

Framebuffer* RenderPass::GetFramebuffer(int index) {
    assert(index < m_framebuffers.size());
    return(m_framebuffers[index]);
}

void RenderPass::Initialize(int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}
