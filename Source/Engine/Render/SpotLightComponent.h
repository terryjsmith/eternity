
#ifndef spotlightcomponent_h
#define spotlightcomponent_h

#include <eternity.h>
#include <Render/LightComponent.h>

#define SPOTLIGHT_TEXTURE_SIZE  512

/**
 * A "spot"/omni-directional light type
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
    void GenerateDepthTexture(Scene* scene);
    
protected:
    // Re-calc matrices
    void RecalculateMatrices();
    
    // A cached set of "look" vectors for the 6 directions
    struct LookVectors {
        vector3 look;
        vector3 up;
        vector3 right;
    };
    
    // Set look vectors
    LookVectors m_vectors[6];
};

#endif
