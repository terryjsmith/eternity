
#ifndef collisionshape_hpp
#define collisionshape_hpp

#include <eternity.h>
#include <Core/GigaObject.h>
#include <Core/DataObject.h>
#include <Core/MathTypes.h>

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class GIGA_API CollisionShape : public DataObject {
public:
    CollisionShape();
    ~CollisionShape();
    
    /**
     * Get collision shape
     */
    btCollisionShape* GetShape() { return m_collisionShape; }
    
    /**
     * Get offset to center
     */
    virtual vector3 GetOffset() { return m_offset; }
    
protected:
    // Collision shape
    btCollisionShape* m_collisionShape;
     vector3 m_offset;
};

#endif
