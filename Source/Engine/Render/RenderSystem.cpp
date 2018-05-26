
#include <Render/RenderSystem.h>

void RenderSystem::Setup(int windowWidth, int windowHeight) {
	std::vector<RenderPass*>::iterator it = m_renderPasses.begin();
	for (; it != m_renderPasses.end(); it++) {
		(*it)->Initialize(windowWidth, windowHeight);
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
