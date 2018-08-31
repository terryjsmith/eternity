
#include <Render/TerrainComponent.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>
#include <Core/DataRecord.h>

void TerrainComponent::Deserialize(DataRecord* record) {
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    m_heightmap = (Texture2D*)resourceSystem->LoadResource(record->Get("heightmap")->AsString(), "Texture2D");
    Texture2D* splat = (Texture2D*)resourceSystem->LoadResource(record->Get("splat")->AsString(), "Texture2D");
    
    m_startX = record->Get("startX")->AsFloat();
    m_startZ = record->Get("startZ")->AsFloat();
    
    float width = m_heightmap->GetWidth();
    
    // Initialize quads
    float newWidth = (float)width / 2.0f;
    
    m_quads[0] = new TerrainQuad();
    m_quads[0]->Initialize(m_startX, m_startZ, newWidth, m_heightmap, splat);
    
    m_quads[1] = new TerrainQuad();
    m_quads[1]->Initialize(m_startX, m_startZ + newWidth, newWidth, m_heightmap, splat);
    
    m_quads[2] = new TerrainQuad();
    m_quads[2]->Initialize(m_startX + newWidth, m_startZ, newWidth, m_heightmap, splat);
    
    m_quads[3] = new TerrainQuad();
    m_quads[3]->Initialize(m_startX + newWidth, m_startZ+ newWidth, newWidth, m_heightmap, splat);
}

void TerrainComponent::Serialize(DataRecord* record) {
    record->Set("heightmap", new Variant(m_heightmap->GetResource()->filename));
    record->Set("startX", new Variant(m_startX));
    record->Set("startZ", new Variant(m_startZ));
    
    if(m_quads[0]) {
        record->Set("splat", new Variant(m_quads[0]->splat->GetResource()->filename));
    }
}

void TerrainComponent::MaybeLoad(BoundingBox *aabb) {
    if(m_quads[0] == 0) {
        return;
    }
    
    m_quads[0]->MaybeLoad(aabb);
    m_quads[1]->MaybeLoad(aabb);
    m_quads[2]->MaybeLoad(aabb);
    m_quads[3]->MaybeLoad(aabb);
    
    this->m_active = true;
}

TerrainQuad* TerrainComponent::GetQuad(int index) {
    GIGA_ASSERT(index > 0, "Invalid range.");
    GIGA_ASSERT(index < 4, "Invalid range.");
    return(m_quads[index]);
}
