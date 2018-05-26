
#ifndef animation_h
#define animation_h

#include <eternity.h>
#include <Render/AnimationFrame.h>

/**
 * A single animation
 */
class GIGA_API Animation {
public:
    Animation() : duration(0), framesPerSecond(0) { }
    virtual ~Animation() = default;
    
    virtual void Update() { }
    
public:
    /**
     * The name of the animation
     */
    std::string name;
    
    /**
     * Duration of the animation in frames
     */
    uint32_t duration;
    
    /**
     * The number of frames per second
     */
    uint32_t framesPerSecond;
    
    /**
     * The frames of this animation
     */
    std::vector<AnimationFrame*> frames;
};

#endif
