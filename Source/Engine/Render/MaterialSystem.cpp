
#include <Render/MaterialSystem.h>
#include <Render/RenderSystem.h>
#include <Render/Color.h>
#include <Core/Application.h>

MaterialSystem::MaterialSystem() {
    memset(m_materials, 0, sizeof(Material*) * MAX_MATERIAL_TEXTURE_SIZE);
    
    // 1024 slots, 4 pixels wide, 3 components per pixel
    memset(m_data, 0, sizeof(float) * MAX_MATERIAL_TEXTURE_SIZE * 4 * 3);
    
    m_texture = 0;
    m_dirty = false;
}

MaterialSystem::~MaterialSystem() {
    if (m_texture) {
        delete m_texture;
        m_texture = 0;
    }
    
    for (int i = 0; i < MAX_MATERIAL_TEXTURE_SIZE; i++) {
        if (m_materials[i]) {
            delete m_materials[i];
        }
    }
}

void MaterialSystem::Initialize() {
    if (m_materials[0]) {
        return;
    }
    
    m_materials[0] = new Material();
    m_materials[0]->ambient = vector4(1, 1, 1, 1);
    m_materials[0]->diffuse = vector4(1, 1, 1, 1);
    m_materials[0]->emissionColor = vector3(0, 0, 0);
    m_materials[0]->emissionStrength = 0.0f;
    m_materials[0]->shininess = 0.0f;
    m_materials[0]->specular = vector4(0, 0, 0, 0);
    
    UpdateMaterialTexture(m_materials[0], 0);
}

int MaterialSystem::AddMaterial(Material* material) {
    m_dirty = true;
    
    for (int i = 0; i < MAX_MATERIAL_TEXTURE_SIZE; i++) {
        if (m_materials[i] == 0) {
            m_materials[i] = material;
            UpdateMaterialTexture(material, i);
            return(i);
        }
    }
    
    assert(false); // BAD: out of material slots
}

int MaterialSystem::GetMaterial(Material* material) {
    for (int i = 0; i < MAX_MATERIAL_TEXTURE_SIZE; i++) {
        if (m_materials[i] == material) {
            return(i);
        }
    }
    
    return(AddMaterial(material));
}

void MaterialSystem::RemoveMaterial(Material* material) {
    m_dirty = true;
    
    for (int i = 0; i < MAX_MATERIAL_TEXTURE_SIZE; i++) {
        if (m_materials[i] == material) {
            m_materials[i] = 0;
            UpdateMaterialTexture(0, i);
        }
    }
}

void MaterialSystem::UpdateMaterialTexture(Material* material, int slot) {
    int offset = slot * 4 * 3;
    
    if (material == 0) {
        // remove
        memset(m_data + offset, 0, sizeof(float) * 4 * 3);
        return;
    }
    
    // Diffuse
    m_data[offset + 0] = material->diffuse.r;
    m_data[offset + 1] = material->diffuse.g;
    m_data[offset + 2] = material->diffuse.b;
    
    // Specular
    m_data[offset + 3] = material->specular.r;
    m_data[offset + 4] = material->specular.g;
    m_data[offset + 5] = material->specular.b;
    
    // Emission color
    m_data[offset + 6] = material->emissionColor.r;
    m_data[offset + 7] = material->emissionColor.g;
    m_data[offset + 8] = material->emissionColor.b;
    
    // Emission strength
    m_data[offset + 9] = material->emissionStrength;
    
    // Shininess
    m_data[offset + 10] = material->shininess;
}

Texture2D* MaterialSystem::GetTexture() {
    if (m_texture == 0) {
        RenderSystem* renderSystem = GetSystem<RenderSystem>();
        m_texture = renderSystem->CreateTexture2D();
        m_texture->Initialize(4, MAX_MATERIAL_TEXTURE_SIZE, COLOR_RGB16F, COLOR_RGB);
    }
    
    // Rebuild our material texture if necessary
    if (m_dirty) {
        // Rebuild
        m_texture->SetData(4, MAX_MATERIAL_TEXTURE_SIZE, COLOR_RGB16F, COLOR_RGB, (void*)m_data);
        m_dirty = false;
    }
    
    return(m_texture);
}
