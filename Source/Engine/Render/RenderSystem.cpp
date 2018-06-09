
#include <Render/RenderSystem.h>

void RenderSystem::Setup(int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    std::vector<RenderPass*>::iterator pi = m_renderPasses.begin();
    for(; pi != m_renderPasses.end(); pi++) {
        (*pi)->Initialize(windowWidth, windowHeight);
    }
}

VertexBuffer* RenderSystem::CreateVertexBuffer() {
	return(new VertexBuffer());
}

VertexType* RenderSystem::CreateVertexType() {
	return(new VertexType());
}

IndexBuffer* RenderSystem::CreateIndexBuffer() {
    return(new IndexBuffer());
}

Texture2D* RenderSystem::CreateTexture2D() {
    return(new Texture2D());
}

Framebuffer* RenderSystem::CreateFramebuffer() {
    return(new Framebuffer());
}

ShaderProgram* RenderSystem::CreateShaderProgram() {
    return(new ShaderProgram());
}
