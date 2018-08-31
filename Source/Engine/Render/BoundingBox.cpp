
#include <Render/BoundingBox.h>

void BoundingBox::Create(vector3 min, vector3 max) {
    this->min = min;
    this->max = max;
    
    points[POINT_NEARBOTTOMLEFT] = vector3(min.x, min.y, min.z);
    points[POINT_NEARBOTTOMRIGHT] = vector3(max.x, min.y, min.z);
    points[POINT_NEARTOPLEFT] = vector3(min.x, max.y, min.z);
    points[POINT_NEARTOPRIGHT] = vector3(max.x, max.y, min.z);
    points[POINT_FARBOTTOMLEFT] = vector3(min.x, min.y, max.z);
    points[POINT_FARBOTTOMRIGHT] = vector3(max.x, min.y, max.z);
    points[POINT_FARTOPLEFT] = vector3(min.x, max.y, max.z);
    points[POINT_FARTOPRIGHT] = vector3(max.x, max.y, max.z);
}

bool BoundingBox::Inside(vector3 point) {
    if (points[POINT_NEARBOTTOMLEFT].x <= point.x &&
        points[POINT_NEARBOTTOMRIGHT].x >= point.x &&
        points[POINT_FARTOPRIGHT].z >= point.z &&
        points[POINT_NEARTOPRIGHT].z <= point.z) {
        // Point is in bounding box
        return(true);
    }
    
    return(false);
}

bool BoundingBox::Intersects(BoundingBox* other) {
    // Check if any point of the other box is inside our bounding box
    for(int i = 0; i < 8; i++) {
        if(this->Inside(other->points[i])) {
            return(true);
        }
    }
    
    // Alternatively, check that we're not totally inside the other box
    if(this->min.x > other->min.x && this->min.y > other->min.y && this->min.z > other->min.z &&
       this->max.x < other->max.x && this->max.y < other->max.y && this->max.z < other->max.z) {
        return(true);
    }
    
    return(false);
}
