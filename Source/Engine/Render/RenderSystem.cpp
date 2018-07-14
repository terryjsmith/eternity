
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

Texture3D* RenderSystem::CreateTexture3D() {
    return(new Texture3D());
}

Framebuffer* RenderSystem::CreateFramebuffer() {
    return(new Framebuffer());
}

ShaderProgram* RenderSystem::CreateShaderProgram() {
    return(new ShaderProgram());
}

void RenderSystem::Update(float delta) {
	// Add components
	std::vector<RenderComponent*>::iterator it = m_components.begin();
	for (; it != m_components.end(); it++) {
		MeshComponent* m = dynamic_cast<MeshComponent*>(*it);
		if (m) {
			m_currentView->AddMesh(m);
		}

		LightComponent* l = dynamic_cast<LightComponent*>(*it);
		if (l) {
			m_currentView->AddLight(l);
		}
	}
}