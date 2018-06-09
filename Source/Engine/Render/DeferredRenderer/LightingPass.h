
#ifndef lightingpass_hpp
#define lightingpass_hpp

#include <eternity.h>
#include <Render/RenderPass.h>

class GIGA_API LightingPass : public RenderPass {
public:
    LightingPass() = default;
    ~LightingPass() = default;
    
    void Initialize(int windowWidth, int windowHeight);
    void Render(Scene* scene);
};

#endif
