
#include <Physics/PhysicsDebugRenderer.h>

PhysicsDebugRenderer::PhysicsDebugRenderer() {
    m_debugMode = 0;
}

PhysicsDebugRenderer::~PhysicsDebugRenderer() {
    
}

void PhysicsDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    //DebugRenderSystem* debugSystem = (DebugRenderSystem*)Application::GetDebugRenderSystem();
    //debugSystem->AddLine(vector3(from.x(), from.y(), from.z()), vector3(to.x(), to.y(), to.z()), vector3(color.x(), color.y(), color.z()), vector3(color.x(), color.y(), color.z()));
}

void PhysicsDebugRenderer::reportErrorWarning(const char *warningString) {
    //Application::Log(ERROR_WARN, (char*)warningString);
}

void PhysicsDebugRenderer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {
    drawLine(PointOnB, PointOnB + normalOnB * distance, color);
}
