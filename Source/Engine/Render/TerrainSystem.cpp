
#include <Render/TerrainSystem.h>
#include <Render/RenderSystem.h>
#include <Core/Application.h>

void TerrainSystem::Initialize() {

}

void TerrainSystem::Update(float delta) {
    // Get the camera from the current view
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    View* currentView = renderSystem->GetCurrentView();
    CameraComponent* camera = currentView->GetCamera();
    
    // Check for visibility
    vector3 cameraPosition = camera->GetTransform()->GetWorldPosition();
    BoundingBox aabb;
    aabb.Create(cameraPosition - m_loadDistance, cameraPosition + m_loadDistance);
    
    // Maybe load
    for(size_t i = 0; i < m_components.size(); i++) {
        m_components[i]->MaybeLoad(&aabb);
    }
}

void TerrainSystem::Shutdown() {
    
}
