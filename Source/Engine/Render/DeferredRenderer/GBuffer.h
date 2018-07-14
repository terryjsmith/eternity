
#ifndef gbuffer_h
#define gbuffer_h

#include <eternity.h>
#include <Render/RenderPass.h>

class GIGA_API GBuffer : public RenderPass {
public:
    GBuffer() = default;
    ~GBuffer() = default;
    
    void Initialize(int windowWidth, int windowHeight);
    void Render(View* scene);
    
protected:
    void RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent);
};

#endif
