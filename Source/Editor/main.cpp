#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <eternity.h>
#include <Core/Application.h>
#include <Core/MessageSystem.h>
#include <Core/MetaSystem.h>
#include <IO/InputSystem.h>
#include <IO/ResourceSystem.h>
#include <Render/MaterialSystem.h>
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Scripting/ScriptingSystem.h>
#include <Physics/PhysicsSystem.h>

int main(int argc, char *argv[]) {
    Application* application = Application::GetInstance();
    World* world = application->GetWorld();

    // Initialize systems
    OpenGLRenderSystem* renderSystem = world->CreateSystem<OpenGLRenderSystem>();
    MaterialSystem* materialSystem = world->CreateSystem<MaterialSystem>();
    ResourceSystem* resourceSystem = world->CreateSystem<ResourceSystem>();
    MessageSystem* messageSystem = world->CreateSystem<MessageSystem>(20);
    InputSystem* inputSystem = world->CreateSystem<InputSystem>();
    MetaSystem* metaSystem = world->CreateSystem<MetaSystem>();
    ScriptingSystem* scriptingSystem = world->CreateSystem<ScriptingSystem>(20);
    PhysicsSystem* physicsSystem = world->CreateSystem<PhysicsSystem>(20);

    QSurfaceFormat glFormat;
    glFormat.setVersion( 4, 5 );
    glFormat.setSamples(4);
    glFormat.setDepthBufferSize(24);
    glFormat.setStencilBufferSize(8);
    glFormat.setSwapInterval(0);
    glFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    glFormat.setProfile( QSurfaceFormat::CoreProfile ); // Requires >=Qt-4.8.0
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);

    world->Initialize();
    application->Initialize();

    MainWindow* w = MainWindow::GetInstance();
    w->showMaximized();

    return a.exec();
}
