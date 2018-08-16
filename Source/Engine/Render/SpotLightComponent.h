
#ifndef spotlightcomponent_hpp
#define spotlightcomponent_hpp

#include <eternity.h>
#include <Render/LightComponent.h>

#define SPOTLIGHT_TEXTURE_SIZE  1024

/**
 * An omni-directional light type
 */
GIGA_CLASS()
class GIGA_API SpotLightComponent : public LightComponent {
public:
    SpotLightComponent() = default;
    ~SpotLightComponent() = default;
    
    GIGA_CLASS_NAME("SpotLightComponent");
    
    /**
     * Initialize
     */
    void Initialize();
    
    /**
     * Create depth textures
     */
    void GenerateDepthTexture(View* scene);
    
    /**
     * Set FOV for spot light
     */
    GIGA_FUNCTION() void SetFOV(float fov) { m_fov = fov; }
    
protected:
     float m_fov;
};

#endif
