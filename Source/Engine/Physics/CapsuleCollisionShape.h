
#ifndef capsulecollisionshape_hpp
#define capsulecollisionshape_hpp

#include <eternity.h>
#include <Physics/CollisionShape.h>

GIGA_CLASS()
class GIGA_API CapsuleCollisionShape : public CollisionShape {
public:
    CapsuleCollisionShape() = default;
    ~CapsuleCollisionShape() = default;
    
    GIGA_CLASS_NAME("CapsuleCollisionShape");
    
    /**
     * Initialize from a radius and height
     */
    GIGA_FUNCTION() void Initialize(float radius, float height);
    
    /**
     * After deserialization
     */
    void PostDeserialize();
    
protected:
    // Height of capsule
     float m_height;
    
    // Radius of capsule
     float m_radius;
};

#endif
