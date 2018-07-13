
#ifndef boxcollisionshape_h
#define boxcollisionshape_h

#include <eternity.h>
#include <Core/MathTypes.h>
#include <Physics/CollisionShape.h>

GIGA_CLASS()
class GIGA_API BoxCollisionShape : public CollisionShape {
public:
    BoxCollisionShape() = default;
    ~BoxCollisionShape() = default;
    
    GIGA_CLASS_NAME("BoxCollisionShape");
    GIGA_CLASS_BODY();
    
    /**
     * Initialize from a set of width/height/depth and position
     */
    GIGA_FUNCTION() void Initialize(vector3 extents, vector3 offset);
    
    /**
     * After deserialization
     */
    void PostDeserialize();
    
protected:
    GIGA_VARIABLE(Serialize) vector3 m_extents;
};

#endif
