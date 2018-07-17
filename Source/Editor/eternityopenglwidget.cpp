#include <Render/OpenGL/OpenGL.h>
#include "eternityopenglwidget.h"
#include <QTimer>
#include <QOpenGLContext>

#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Render/OpenGL/OpenGLRenderSystem.h>

EternityOpenGLWidget::EternityOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    m_framebufferWidth = m_framebufferHeight = 0;
    m_scene = 0;
    m_initialized = false;
}

void EternityOpenGLWidget::initializeGL() {
    // Get resource system
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    resourceSystem->AddSearchPath("");
    resourceSystem->AddSearchPath("resources/shaders");
    resourceSystem->AddSearchPath("resources/textures");
    resourceSystem->AddSearchPath("resources/models");
    resourceSystem->AddSearchPath("resources/scripts");

#ifdef WIN32
    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        return;
    }
#endif

    m_scene = new View();

    m_updateTimer = new QTime;
    m_updateTimer->start();

    CameraComponent* camera = new CameraComponent();
    m_scene->SetCamera(camera);
    camera->GetTransform()->SetWorldPosition(vector3(0, 1.5, 4));

    QOpenGLWidget::initializeGL();
}

void EternityOpenGLWidget::resizeGL(int w, int h) {
    QOpenGLContext* context = this->context();
    QSurfaceFormat fmt = context->format();

    OpenGLRenderSystem* renderSystem = GetSystem<OpenGLRenderSystem>();
    renderSystem->SetCurrentView(m_scene);

    if(m_initialized == false) {
        m_initialized = true;
        return;
    }

    // Make sure we have an OpenGL 4+ context
    int majorVersion = fmt.majorVersion();
    int minorVersion = fmt.minorVersion();
    assert(majorVersion == 4);
    int depth = fmt.depthBufferSize();
    int samples = fmt.samples();
    QSurfaceFormat::SwapBehavior beh = fmt.swapBehavior();

    // Get the actual render width/height (retina)
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_framebufferWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_framebufferHeight);

    renderSystem->Setup(m_framebufferWidth, m_framebufferHeight);

    unsigned int framebuffer = defaultFramebufferObject();
    renderSystem->SetDefaultFramebuffer(framebuffer);

    QOpenGLWidget::resizeGL(w, h);
}

void EternityOpenGLWidget::paintGL() {
    float delta = m_updateTimer->elapsed() / 1000.0f;
    m_updateTimer->restart();

    if(m_framebufferWidth == 0) {
        return;
    }

    World* world = Application::GetInstance()->GetWorld();
    world->Update(delta);

    OpenGLRenderSystem* renderSystem = GetSystem<OpenGLRenderSystem>();
    renderSystem->Render();

    QTimer::singleShot(10, this, SLOT(update()));
}
