
#ifndef materialsystem_h
#define materialsystem_h

#include <eternity.h>
#include <Core/System.h>
#include <Render/Material.h>
#include <Render/Texture2D.h>

#define MAX_MATERIAL_TEXTURE_SIZE	1024

/**
 * Internal system for managing materials
 */
class GIGA_API MaterialSystem : public System {
public:
    MaterialSystem();
    ~MaterialSystem();
    
    GIGA_CLASS_NAME("MaterialSystem");
    
    /**
     * Initialize the system (creates a default material 0)
     */
    void Initialize();
    
    /**
     * System overrides
     */
    void Update(float delta) { }
    void Shutdown() { }
    
    /**
     * Add a new material, returning the slot ID
     */
    int AddMaterial(Material* material);
    
    /**
     * Get the slot ID of a material, add it if it does not exist
     */
    int GetMaterial(Material* material);
    
    /**
     * Remove a material from our system
     */
    void RemoveMaterial(Material* material);
    
    /**
     * Get default system material
     */
    Material* GetDefaultMaterial() { return m_materials[0]; }
    
    /**
     * Get the material texture
     */
    Texture2D* GetTexture();
    
protected:
    /**
     * Add a material to our internal texture
     */
    void UpdateMaterialTexture(Material* material, int slot);
    
protected:
    // An array of materials
    Material* m_materials[MAX_MATERIAL_TEXTURE_SIZE];
    
    // Our material lookup texture
    Texture2D* m_texture;
    
    // Whether this material texture is dirty and needs to be re-built
    bool m_dirty;
    
    // Our internal texture data
    float m_data[MAX_MATERIAL_TEXTURE_SIZE * 12];
};

#endif
