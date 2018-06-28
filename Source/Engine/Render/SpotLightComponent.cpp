
#include <Render/SpotLightComponent.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <Core/Application.h>

void SpotLightComponent::Initialize() {
    m_passes = 6;
    m_type = LIGHT_SPOT;
    
    // Get link to render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Create cubemap shadow texture
    m_depthTexture = renderSystem->CreateTexture3D();
    m_depthTexture->Initialize(SPOTLIGHT_TEXTURE_SIZE, SPOTLIGHT_TEXTURE_SIZE, COLOR_DEPTH_COMPONENT, COLOR_DEPTH_COMPONENT);
    
    // Initialize depth pass
    m_depthPass = new DepthPass();
    m_depthPass->Initialize(SPOTLIGHT_TEXTURE_SIZE, SPOTLIGHT_TEXTURE_SIZE);
    
    // Initialize "camera"
    m_camera = new CameraComponent();
    m_camera->SetNear(0.1f);
    
    // Right
    m_vectors[0].look = vector3(1, 0, 0);
    m_vectors[0].up = vector3(0, -1, 0);
    m_vectors[0].right = vector3(0, 0, -1);
    
    // Left
    m_vectors[1].look = vector3(-1, 0, 0);
    m_vectors[1].up = vector3(0, -1, 0);
    m_vectors[1].right = vector3(0, 0, 1);
    
    // Up
    m_vectors[2].look = vector3(0, 1, 0);
    m_vectors[2].up = vector3(0, 0, 1);
    m_vectors[2].right = vector3(1, 0, 0);
    
    // Down
    m_vectors[3].look = vector3(0, -1, 0);
    m_vectors[3].up = vector3(0, 0, -1);
    m_vectors[3].right = vector3(1, 0, 0);
    
    // Backward
    m_vectors[4].look = vector3(0, 0, 1);
    m_vectors[4].up = vector3(0, -1, 0);
    m_vectors[4].right = vector3(1, 0, 0);
    
    // Forward
    m_vectors[5].look = vector3(0, 0, -1);
    m_vectors[5].up = vector3(0, -1, 0);
    m_vectors[5].right = vector3(-1, 0, 0);
}

void SpotLightComponent::GenerateDepthTexture(Scene* scene) {
    // Get our 3D texture
    Texture3D* texture = (Texture3D*)m_depthTexture;
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Get world position
    vector3 position = GetTransform()->GetWorldPosition();
    
    // Update "camera"
    m_camera->GetTransform()->SetWorldPosition(position);
    m_camera->SetFar(m_attenuation);
    m_camera->SetFOV(90.0f);
    m_camera->SetAspectRatio(1.0f);
    
    // Loop over each pass
    for(int i = 0; i < m_passes; i++) {
        m_depthPass->SetTexture(texture, texture->GetTarget(i));
        
        // Calculate frustum and matrices
        m_camera->GetTransform()->SetLook(m_vectors[i].look);
        m_camera->GetTransform()->SetUp(m_vectors[i].up);
        m_camera->GetTransform()->SetRight(m_vectors[i].right);
        
        // Set "camera" and draw
        m_depthPass->SetView(m_camera);
        m_depthPass->Render(scene);
        
        if(i == 3) {
            //scene->SetCamera(m_camera);
            //break;
        }
    }
    
    //texture->Save("depth_out.bmp");
}

void SpotLightComponent::RecalculateMatrices() {
    
}
