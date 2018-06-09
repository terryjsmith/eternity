
#include <Render/DeferredRenderer/LightingPass.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <Core/Application.h>

void LightingPass::Initialize(int windowWidth, int windowHeight) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    Framebuffer* lightingFramebuffer = renderSystem->CreateFramebuffer();
    lightingFramebuffer->Initialize();
    
    // Create our textures
    Texture2D* diffuseTexture = renderSystem->CreateTexture2D();
    diffuseTexture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    lightingFramebuffer->AddTexture(diffuseTexture, FRAMEBUFFER_SLOT_0);
    
    m_framebuffers.push_back(lightingFramebuffer);
}

void LightingPass::Render(Scene* scene) {
    m_framebuffers[0]->Bind();
    
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->Clear(COLOR_BUFFER_BIT);
    
    m_framebuffers[0]->Unbind();
}
