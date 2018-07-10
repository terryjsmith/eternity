
#include <Render/SpotLightComponent.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <Core/Application.h>

void SpotLightComponent::Initialize() {
    m_passes = 1;
    m_type = LIGHT_SPOT;
    m_fov = 45.0f;
    
    // Get link to render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Create cubemap shadow texture
    m_depthTexture = renderSystem->CreateTexture2D();
    m_depthTexture->Initialize(SPOTLIGHT_TEXTURE_SIZE, SPOTLIGHT_TEXTURE_SIZE, COLOR_DEPTH_COMPONENT, COLOR_DEPTH_COMPONENT);
    
    // Initialize depth pass
    m_depthPass = new DepthPass();
    m_depthPass->Initialize(SPOTLIGHT_TEXTURE_SIZE, SPOTLIGHT_TEXTURE_SIZE);
    
    // Initialize "camera"
    m_camera = new CameraComponent();
    m_camera->SetNear(0.1f);
}

void SpotLightComponent::GenerateDepthTexture(Scene* scene) {
    // Get our 2D texture
    Texture2D* texture = dynamic_cast<Texture2D*>(m_depthTexture);
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Get world position
    vector3 position = GetTransform()->GetWorldPosition();
    
    // Update "camera"
    m_camera->GetTransform()->SetWorldPosition(position);
    m_camera->SetFar(m_attenuation);
    m_camera->SetFOV(m_fov);
    m_camera->SetAspectRatio(1.0f);
    //m_camera->GetTransform()->SetUp(vector3(0, -1, 0));
    
    // Single pass
    m_depthPass->SetTexture(texture);
        
    // Set "camera" and draw
    m_depthPass->SetView(m_camera);
    m_depthPass->Render(scene);
    
    //m_camera->GetTransform()->SetUp(vector3(0, 1, 0));
    //texture->Save("spotlight.bmp");
}
