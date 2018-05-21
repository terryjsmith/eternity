
#ifndef vector3_h
#define vector3_h

#include <eternity.h>
#include <Core/MathTypes.h>
#include <Core/GigaObject.h>

GIGA_CLASS()
class GIGA_API Vector3 : public GigaObject {
public:
    Vector3() : x(0), y(0), z(0) { }
    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3(vector3 rhs) : x(rhs.x), y(rhs.y), z(rhs.z) { }
    ~Vector3() = default;
    
    GIGA_CLASS_NAME("Vector3");
    
public:
    GIGA_VARIABLE(Get, Set) float x;
    GIGA_VARIABLE(Get, Set) float y;
    GIGA_VARIABLE(Get, Set) float z;
};

#endif
