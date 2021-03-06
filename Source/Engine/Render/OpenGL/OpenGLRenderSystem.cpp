
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Render/OpenGL/OpenGLVertexBuffer.h>
#include <Render/OpenGL/OpenGLIndexBuffer.h>
#include <Render/OpenGL/OpenGLVertexType.h>
#include <Render/OpenGL/OpenGL.h>
#include <Render/OpenGL/OpenGLTexture2D.h>
#include <Render/OpenGL/OpenGLTexture3D.h>
#include <Render/OpenGL/OpenGLFramebuffer.h>
#include <Render/OpenGL/OpenGLShaderProgram.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

void OpenGLRenderSystem::Initialize() {
	m_renderPasses.clear();

    // GBuffer pass
	if (m_gbufferRenderPass) {
		delete m_gbufferRenderPass;
		m_gbufferRenderPass = 0;
	}

	m_gbufferRenderPass = new GBuffer();
    m_renderPasses.push_back(m_gbufferRenderPass);

    // Combination pass
	if (m_combineRenderPass) {
		delete m_combineRenderPass;
		m_combineRenderPass = 0;
	}

    m_combineRenderPass = new CombinePass();
    m_renderPasses.push_back(m_combineRenderPass);
    
    // Lighting pass
	if (m_lightingRenderPass) {
		delete m_lightingRenderPass;
		m_lightingRenderPass = 0;
	}

    m_lightingRenderPass = new LightingPass();
    m_renderPasses.push_back(m_lightingRenderPass);
    
    // Terrain pass
    if(m_terrainRenderPass) {
        delete m_terrainRenderPass;
        m_terrainRenderPass = 0;
    }
    
    m_terrainRenderPass = new TerrainPass();
    m_renderPasses.push_back(m_terrainRenderPass);
    
    // Set our texture type to OpenGL
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    resourceSystem->RegisterResourceType<OpenGLTexture2D>("Texture2D");
}

Texture2D* OpenGLRenderSystem::GetIndexTexture() {
	Texture2D* texture = (Texture2D*)m_gbufferRenderPass->GetFramebuffer(0)->GetTexture(3);
	return(texture);
}

Texture2D* OpenGLRenderSystem::GetPositionTexture() {
	Texture2D* texture = (Texture2D*)m_gbufferRenderPass->GetFramebuffer(0)->GetTexture(1);
	return(texture);
}

Texture2D* OpenGLRenderSystem::GetNormalTexture() {
	Texture2D* texture = (Texture2D*)m_gbufferRenderPass->GetFramebuffer(0)->GetTexture(2);
	return(texture);
}

Texture2D* OpenGLRenderSystem::GetLightingTexture() {
	Texture2D* texture = (Texture2D*)m_lightingRenderPass->GetFramebuffer(0)->GetTexture(0);
	return(texture);
}

Texture2D* OpenGLRenderSystem::GetDiffuseTexture() {
	Texture2D* texture = (Texture2D*)m_gbufferRenderPass->GetFramebuffer(0)->GetTexture(0);
	return(texture);
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

Texture3D* OpenGLRenderSystem::CreateTexture3D() {
    return(new OpenGLTexture3D());
}

Framebuffer* OpenGLRenderSystem::CreateFramebuffer() {
    return(new OpenGLFramebuffer());
}

ShaderProgram* OpenGLRenderSystem::CreateShaderProgram() {
    return(new OpenGLShaderProgram());
}

void OpenGLRenderSystem::Render() {
    /**
     * Depth pre-pass
     */
    
    /**
     * Depth pass (per light)
     */
    std::vector<LightComponent*> lights = m_currentView->GetLights();
    std::vector<LightComponent*>::iterator li = lights.begin();
    for(; li != lights.end(); li++) {
        (*li)->GenerateDepthTexture(m_currentView);
    }
    
    /**
     * G-buffer pass
     */
	m_gbufferRenderPass->Render(m_currentView);
    Framebuffer* gbuffer = m_gbufferRenderPass->GetFramebuffer(0);
    
    /**
     * Terrain pass
     */
    m_terrainRenderPass->SetFramebuffer(gbuffer);
    m_terrainRenderPass->Render(m_currentView);
    
    /**
     * Lighting
     */
    m_lightingRenderPass->SetDiffuseTexture((Texture2D*)gbuffer->GetTexture(0));
    m_lightingRenderPass->SetPositionTexture((Texture2D*)gbuffer->GetTexture(1));
    m_lightingRenderPass->SetNormalTexture((Texture2D*)gbuffer->GetTexture(2));
    m_lightingRenderPass->SetMaterialTexture((Texture2D*)gbuffer->GetTexture(3));
    
    m_lightingRenderPass->Render(m_currentView);
    
    /**
     * Combine pass
     */
    m_combineRenderPass->SetDiffuseTexture((Texture2D*)gbuffer->GetTexture(0));
    m_combineRenderPass->SetPositionTexture((Texture2D*)gbuffer->GetTexture(1));
    m_combineRenderPass->SetNormalTexture((Texture2D*)gbuffer->GetTexture(2));
    m_combineRenderPass->SetLightingTexture((Texture2D*)m_lightingRenderPass->GetFramebuffer(0)->GetTexture(0));
    
    m_combineRenderPass->Render(m_currentView);
    
    /**
     * Post-processing
     */
}

void OpenGLRenderSystem::Clear(int bitmask) {
    GL_CHECK(glClear(bitmask));
}

void OpenGLRenderSystem::SetClearColor(vector4 color) {
    GL_CHECK(glClearColor(color.r, color.g, color.b, color.a));
}

void OpenGLRenderSystem::Draw(int type, int elements) {
    GL_CHECK(glDrawArrays(type, 0, elements));
}

void OpenGLRenderSystem::DrawIndexed(int type, int elements) {
    GL_CHECK(glDrawElements(type, elements, GL_UNSIGNED_INT, 0));
}

void OpenGLRenderSystem::EnableDepthTest(int type) {
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(type));
}

void OpenGLRenderSystem::DisableDepthTest() {
    GL_CHECK(glDisable(GL_DEPTH_TEST));
}

void OpenGLRenderSystem::EnableBlending() {
    GL_CHECK(glEnable(GL_BLEND));
}

void OpenGLRenderSystem::DisableBlending() {
    GL_CHECK(glDisable(GL_BLEND));
}

void OpenGLRenderSystem::SetBlendFunc(int sourceFactor, int destFactor) {
    GL_CHECK(glBlendFunc(sourceFactor, destFactor));
}

void OpenGLRenderSystem::SetViewport(int width, int height) {
    GL_CHECK(glViewport(0, 0, width, height));
}

void OpenGLRenderSystem::SetDrawBuffer(int buffer) {
    GL_CHECK(glDrawBuffer(buffer));
}

void OpenGLRenderSystem::UseDefaultFramebuffer() {
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer));
}
