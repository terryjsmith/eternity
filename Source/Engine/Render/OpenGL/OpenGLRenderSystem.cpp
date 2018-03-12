
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Render/OpenGL/OpenGLVertexBuffer.h>
#include <Render/OpenGL/OpenGLVertexType.h>
#include <Render/OpenGL/OpenGL.h>

void OpenGLRenderSystem::Initialize() {
	m_deferredRenderPass = new OpenGLDeferredRenderPass();
	m_renderPasses.push_back(m_deferredRenderPass);
}

VertexBuffer* OpenGLRenderSystem::CreateVertexBuffer() {
	return(new OpenGLVertexBuffer());
}

VertexType* OpenGLRenderSystem::CreateVertexType() {
	return(new OpenGLVertexType());
}

void OpenGLRenderSystem::Render(Scene* scene) {
	m_deferredRenderPass->Render(scene);
}