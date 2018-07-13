
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
    GIGA_CLASS_BODY();
    
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
    GIGA_VARIABLE(Serialize) float m_height;
    
    // Radius of capsule
    GIGA_VARIABLE(Serialize) float m_radius;
};

#endif
