
#ifndef boundingbox_h
#define boundingbox_h

#include <eternity.h>
#include <Core/MathTypes.h>

/**
 * An axis-aligned bounding box
 */
class GIGA_API BoundingBox {
public:
    BoundingBox() = default;
    ~BoundingBox() = default;
    
    enum {
        POINT_NEARTOPLEFT = 0,
        POINT_NEARTOPRIGHT,
        POINT_NEARBOTTOMLEFT,
        POINT_NEARBOTTOMRIGHT,
        POINT_FARTOPLEFT,
        POINT_FARTOPRIGHT,
        POINT_FARBOTTOMLEFT,
        POINT_FARBOTTOMRIGHT
    };
    
    /**
     * Create a bounding box from min and max coords
     */
    void Create(vector3 min, vector3 max);
    
    /**
     * Check to see if a point is inside the box
     */
    bool Inside(vector3 point);
    
public:
    // The 8 points that make up this box
    vector3 points[8];
    
    // Min/max of each coordinate
    vector3 min, max;
};

#endif
