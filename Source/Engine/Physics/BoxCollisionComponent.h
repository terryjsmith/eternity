
#ifndef boxcollidercomponent_h
#define boxcollidercomponent_h

#include <eternity.h>
#include <Physics/RigidBodyComponent.h>

GIGA_CLASS()
class GIGA_API BoxCollisionComponent : public RigidBodyComponent {
public:
    BoxCollisionComponent() = default;
    ~BoxCollisionComponent() = default;
    
    GIGA_CLASS_NAME("BoxCollisionComponent");
    GIGA_CLASS_BODY();
    
    /**
     * Initialize with an oriented bounding box and mass (0 for static)
     */
    GIGA_FUNCTION() void Initialize(vector3 extents, vector3 position, vector3 offset, float mass);
};

#endif
