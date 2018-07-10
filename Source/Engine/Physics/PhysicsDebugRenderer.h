
#ifndef physicsdebugrenderer_h
#define physicsdebugrenderer_h

#include <eternity.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class GIGA_API PhysicsDebugRenderer : public btIDebugDraw {
public:
    PhysicsDebugRenderer();
    ~PhysicsDebugRenderer();
public:
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
    
    void reportErrorWarning(const char* warningString);
    
    void setDebugMode(int debugMode) { m_debugMode = debugMode; }
    int getDebugMode() const { return m_debugMode; }
    
    void draw3dText(const btVector3& location,const char* textString) { }
    
protected:
    int m_debugMode;
};

#endif
