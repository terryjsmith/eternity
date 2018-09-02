
#ifndef terrainpass_hpp
#define terrainpass_hpp

#include <eternity.h>
#include <Render/RenderPass.h>

class GIGA_API TerrainPass : public RenderPass {
public:
    TerrainPass() = default;
    ~TerrainPass() = default;
    
    void Initialize(int windowWidth, int windowHeight);
    void Render(View* scene);
    
    /**
     * Set framebuffer
     */
    void SetFramebuffer(Framebuffer* framebuffer);
    
protected:
    void RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent);
};

#endif