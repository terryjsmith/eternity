#include <Render/OpenGL/OpenGL.h>
#include "eternityopenglwidget.h"
#include <QTimer>

#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Render/OpenGL/OpenGLRenderSystem.h>

EternityOpenGLWidget::EternityOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    m_framebufferWidth = m_framebufferHeight = 0;
    m_scene = 0;
}

void EternityOpenGLWidget::initializeGL() {
    // Get resource system
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    resourceSystem->AddSearchPath("resources/shaders");
    resourceSystem->AddSearchPath("resources/images");
    resourceSystem->AddSearchPath("resources/models");
    resourceSystem->AddSearchPath("resources/scripts");

#ifdef WIN32
    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        return;
    }
#endif

    m_scene = new Scene();

    CameraComponent* camera = new CameraComponent();
    m_scene->SetCamera(camera);
}

void EternityOpenGLWidget::resizeGL(int w, int h) {
    // Get the actual render width/height (retina)
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_framebufferHeight);

    OpenGLRenderSystem* renderSystem = GetSystem<OpenGLRenderSystem>();
    renderSystem->Setup(m_framebufferWidth, m_framebufferHeight);
    renderSystem->SetCurrentScene(m_scene);
}

void EternityOpenGLWidget::paintGL() {
    OpenGLRenderSystem* renderSystem = GetSystem<OpenGLRenderSystem>();
    renderSystem->Render();

    QTimer::singleShot(10, this, SLOT(update()));
}
