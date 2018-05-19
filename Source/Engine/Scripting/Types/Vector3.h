
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
    
    GIGA_FUNCTION() float X() { return x; }
    GIGA_FUNCTION() float Y() { return y; }
    GIGA_FUNCTION() float Z() { return z; }
    
    GIGA_FUNCTION() void SetX(float x) { this->x = x; }
    GIGA_FUNCTION() void SetY(float y) { this->y = y; }
    GIGA_FUNCTION() void SetZ(float z) { this->z = z; }
    
public:
    float x, y, z;
};

#endif
