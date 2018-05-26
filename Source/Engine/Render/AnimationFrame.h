
#ifndef animationframe_h
#define animationframe_h

#include <eternity.h>
#include <Render/AnimationTransform.h>

/**
 * A frame of an animation
 */
class GIGA_API AnimationFrame {
public:
    AnimationFrame() = default;
    ~AnimationFrame() = default;
    
public:
    /**
     * The timestamp of this transformation (in ticks)
     */
    uint32_t time;
    
    /**
     * The objects affected by this and how
     */
    std::vector<AnimationTransform*> transforms;
};

#endif
