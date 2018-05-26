
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Render/OpenGL/OpenGLVertexBuffer.h>
#include <Render/OpenGL/OpenGLIndexBuffer.h>
#include <Render/OpenGL/OpenGLVertexType.h>
#include <Render/OpenGL/OpenGL.h>
#include <Render/OpenGL/OpenGLTexture2D.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

void OpenGLRenderSystem::Initialize() {
	m_deferredRenderPass = new OpenGLDeferredRenderPass();
	m_renderPasses.push_back(m_deferredRenderPass);
    
    // Set our texture type to OpenGL
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    resourceSystem->RegisterResourceType<OpenGLTexture2D>("Texture2D");
}

VertexBuffer* OpenGLRenderSystem::CreateVertexBuffer() {
	return(new OpenGLVertexBuffer());
}

IndexBuffer* OpenGLRenderSystem::CreateIndexBuffer() {
    return(new OpenGLIndexBuffer());
}

VertexType* OpenGLRenderSystem::CreateVertexType() {
	return(new OpenGLVertexType());
}

Texture2D* OpenGLRenderSystem::CreateTexture2D() {
    return(new OpenGLTexture2D());
}

void OpenGLRenderSystem::Render() {
	m_deferredRenderPass->Render(m_currentScene);
}

