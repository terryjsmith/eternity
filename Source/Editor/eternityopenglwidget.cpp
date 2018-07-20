#include <Render/OpenGL/OpenGL.h>
#include "eternityopenglwidget.h"
#include <QTimer>
#include <QOpenGLContext>
#include "mainwindow.h"

#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Render/OpenGL/OpenGLRenderSystem.h>
#include <Render/ArcballCameraComponent.h>

EternityOpenGLWidget::EternityOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    m_framebufferWidth = m_framebufferHeight = 0;
    m_scene = 0;
    m_initialized = false;
    m_width = m_height = 0;
    m_currentMesh = 0;
    m_editorState = 0;
    m_isDragging = false;
    m_isMoving = false;
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

    ArcballCameraComponent* camera = new ArcballCameraComponent();
    m_scene->SetCamera(camera);
    camera->GetTransform()->SetWorldPosition(vector3(0, 1.5, 4));

    QOpenGLWidget::initializeGL();
}

void EternityOpenGLWidget::resizeGL(int w, int h) {
    QOpenGLContext* context = this->context();
    QSurfaceFormat fmt = context->format();

    m_width = w;
    m_height = h;

    this->setMouseTracking(true);

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

void EternityOpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    MainWindow* window = MainWindow::GetInstance();

    // Set focus
    setFocus();

    if(m_isDragging) {
        m_isDragging = false;
    }

    if(m_isMoving) {
        m_isMoving = false;
    }

    if(event->button() == Qt::MouseButton::LeftButton) {
        // Get the position of the mouse click
        QPoint pos = event->pos();

        // Get the index buffer
        RenderSystem* rs = GetSystem<RenderSystem>();
        Texture2D* indexTexture = rs->GetIndexTexture();

        if(m_scene == 0) return;

        int scaleFactor = m_framebufferWidth / m_width;

        float* data = (float*)malloc(sizeof(float) * m_framebufferWidth * m_framebufferHeight * 3);
        indexTexture->Bind(0);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
        indexTexture->Unbind();

        // Get the clicked index
        int dindex = (((m_framebufferHeight - (pos.ry() * scaleFactor)) * m_framebufferWidth) + (pos.rx() * scaleFactor)) * 3;
        float index = data[dindex];
        free(data);

        if(index < 0) {
            m_currentMesh = 0;
            m_currentLight = 0;
            return;
        }

        // Get the clicked item
        std::vector<MeshComponent*> renderList = m_scene->GetMeshes();

        m_currentMesh = renderList[(int)index];
        m_currentLight = 0;
        m_lastPosition = vector3(m_currentMesh->GetTransform()->GetWorldPosition());

        // TODO: bool selected = window->SetSelectedEntity(m_currentMesh);

        //if(selected == false) {
            // Check whether this was a light renderable
        //}
    }

    QOpenGLWidget::mouseReleaseEvent(event);
}

void EternityOpenGLWidget::mousePressEvent(QMouseEvent *event) {
    MainWindow* window = MainWindow::GetInstance();

    // Set focus
    setFocus();

    // If we were moving something, "drop" it here
    if(m_editorState) {
        if(event->button() == Qt::MouseButton::RightButton) {
            // Cancel current movement
            if(m_currentMesh) {
                m_currentMesh->GetTransform()->SetWorldPosition(m_lastPosition);
            }
        }

        m_editorState = 0;
    }

    if(m_scene == 0) return;

    if(event->button() == Qt::MouseButton::MiddleButton) {
        // Get the position of the mouse click
        QPoint pos = event->pos();

        // Get the index buffer
        RenderSystem* rs = GetSystem<RenderSystem>();
        Texture2D* positionTexture = rs->GetPositionTexture();

        ArcballCameraComponent* camera = (ArcballCameraComponent*)m_scene->GetCamera();
        int scaleFactor = m_framebufferWidth / m_width;

        float* data = (float*)malloc(sizeof(float) * m_framebufferWidth * m_framebufferHeight * 3);
        positionTexture->Bind(0);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
        positionTexture->Unbind();

        // Get the clicked index
        int index = (((m_framebufferHeight - (pos.ry() * scaleFactor)) * m_framebufferWidth) + (pos.rx() * scaleFactor)) * 3;
        float x = data[index + 0];
        float y = data[index + 1];
        float z = data[index + 2];

        vector4 worldVector;
        if(x == 0 && y == 0 && z == 0) {
            // If we clicked on "black space", put the world vector some units in front of us
            worldVector = vector4(camera->GetTransform()->GetLook() * 15.0f, 1.0f);
        }
        else {
            matrix4 invView = glm::inverse(camera->GetViewMatrix());
            worldVector = invView * vector4(x, y, z, 1.0f);
        }


        free(data);

        // Start a potential drag operation by setting the target point to the distance from camera to "target", but along the look
        vector3 cameraPos = camera->GetTransform()->GetWorldPosition();
        float distance = glm::length(cameraPos - vector3(worldVector));
        vector3 targetPos = camera->GetTransform()->GetLook() * distance;
        camera->SetTarget(cameraPos + targetPos);

        m_isDragging = true;
    }

    if(event->button() == Qt::MouseButton::LeftButton) {
        m_isMoving = true;
    }

    QOpenGLWidget::mousePressEvent(event);
}

void EternityOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint pos = event->pos();

    QPoint diff = m_lastMousePos - event->pos();
    if(diff.manhattanLength() < 3) {
        diff = { 0, 0 };
    }

    ArcballCameraComponent* camera = (ArcballCameraComponent*)m_scene->GetCamera();

    // If we are dragging, move the camera
    if(m_isDragging) {
        float rotateFactor = 0.2f;
        camera->RotateX((float)-diff.y() * rotateFactor); // RotateX/RotateY
        camera->RotateY((float)-diff.x() * rotateFactor);
    }

    if(m_isMoving) {
        float moveFactor = 0.01f;
        camera->GetTransform()->Move(camera->GetTransform()->GetRight() * (float)diff.x() * moveFactor);
        camera->GetTransform()->Move(camera->GetTransform()->GetUp() * (float)-diff.y() * moveFactor);
    }

    if(m_editorState && m_currentMesh) {
        MainWindow* window = MainWindow::GetInstance();
        ArcballCameraComponent* camera = (ArcballCameraComponent*)m_scene->GetCamera();
        vector3 position = m_currentMesh->GetTransform()->GetWorldPosition();

        vector3 cameraPos = camera->GetTransform()->GetWorldPosition();
        vector3 cameraUp = camera->GetTransform()->GetUp();
        vector3 cameraRight = camera->GetTransform()->GetRight();
        float cameraFOV = camera->GetFOV();
        float cameraAspect = camera->GetAspectRatio();

        RenderSystem* rs = GetSystem<RenderSystem>();
        int width, height;
        width = m_framebufferWidth;
        height = m_framebufferHeight;

        // Normalized coords
        float x = (2.0f * pos.rx()) / (float)width - 1.0f;
        float y = 1.0f - (2.0f * pos.ry()) / (float)height;

        // Get the width and height of the "viewport" at the distance clicked
        vector3 dvec = position - cameraPos;
        float distance = glm::length(dvec);
        float farHeight = 2 * tan(cameraFOV / 2) * distance;
        float farWidth = farHeight * cameraAspect;

        vector3 newPosition = m_lastPosition + (x * farWidth * cameraRight) + (y * farHeight * cameraUp);

        // If the user if holding Control snap to "grid"
        if(m_keys[Qt::Key::Key_Control]) {
            newPosition = vector3((int)newPosition.x, (int)newPosition.y, (int)newPosition.z);
        }

        if(m_editorState == EDITORSTATE_TRANSLATE) {
            m_currentMesh->GetTransform()->SetWorldPosition(newPosition);
        }

        if(m_editorState == EDITORSTATE_TRANSLATE_X) {
            m_currentMesh->GetTransform()->SetWorldPosition(vector3(newPosition.x, position.y, position.z));
        }

        if(m_editorState == EDITORSTATE_TRANSLATE_Y) {
            m_currentMesh->GetTransform()->SetWorldPosition(vector3(position.x, newPosition.y, position.z));
        }

        if(m_editorState == EDITORSTATE_TRANSLATE_Z) {
            m_currentMesh->GetTransform()->SetWorldPosition(vector3(position.x, position.y, newPosition.z));
        }

        // Check to see if we have a light to move (light renderable)
        if(m_currentLight) {

        }
    }

    m_lastMousePos = pos;
}

void EternityOpenGLWidget::wheelEvent(QWheelEvent* event) {
    if(m_editorState == 0) {
        ArcballCameraComponent* camera = (ArcballCameraComponent*)m_scene->GetCamera();
        vector3 look = camera->GetTransform()->GetLook();
        float delta = event->delta() / 8.0f / 15.0f;
        camera->GetTransform()->Move(look * delta);
    }
}


void EternityOpenGLWidget::keyPressEvent(QKeyEvent * event) {
    MainWindow* window = MainWindow::GetInstance();
    QStatusBar* statusBar = window->GetStatusBar();

    // Record key state
    m_keys[event->key()] = true;

    if(event->key() == Qt::Key::Key_G) {
        m_editorState = EDITORSTATE_TRANSLATE;
        QCursor::setPos(mapToGlobal(QPoint(size().width() / 2, size().height() / 2)));
        statusBar->showMessage("Current action: Translate");
    }

    if((event->key() == Qt::Key::Key_X) && (m_editorState == EDITORSTATE_TRANSLATE)) {
        if(m_currentMesh) {
            // Start tracking movements
            m_editorState = EDITORSTATE_TRANSLATE_X;
            vector3 currentPosition = m_currentMesh->GetTransform()->GetWorldPosition();
            m_currentMesh->GetTransform()->SetWorldPosition(vector3(currentPosition.x, m_lastPosition.y, m_lastPosition.z));
            statusBar->showMessage("Current action: Translate X");
        }
    }

    if((event->key() == Qt::Key::Key_Y) && (m_editorState == EDITORSTATE_TRANSLATE)) {
        if(m_currentMesh) {
            // Start tracking movements
            m_editorState = EDITORSTATE_TRANSLATE_Y;
            vector3 currentPosition = m_currentMesh->GetTransform()->GetWorldPosition();
            m_currentMesh->GetTransform()->SetWorldPosition(vector3(m_lastPosition.x, currentPosition.y, m_lastPosition.z));
            statusBar->showMessage("Current action: Translate Y");
        }
    }

    if((event->key() == Qt::Key::Key_Z) && (m_editorState == EDITORSTATE_TRANSLATE)) {
        if(m_currentMesh) {
            // Start tracking movements
            m_editorState = EDITORSTATE_TRANSLATE_Z;
            vector3 currentPosition = m_currentMesh->GetTransform()->GetWorldPosition();
            m_currentMesh->GetTransform()->SetWorldPosition(vector3(m_lastPosition.x, m_lastPosition.y, currentPosition.z));
            statusBar->showMessage("Current action: Translate Z");
        }
    }

    if(event->key() == Qt::Key::Key_Escape) {
        if(m_editorState) {
            if(m_currentMesh) {
                m_currentMesh->GetTransform()->SetWorldPosition(m_lastPosition);
            }
        }
        else {
            // Select no entity
        }

        m_editorState = 0;
        statusBar->clearMessage();
    }

    QOpenGLWidget::keyPressEvent(event);
}

void EternityOpenGLWidget::keyReleaseEvent(QKeyEvent * event) {
    m_keys[event->key()] = false;
    QOpenGLWidget::keyReleaseEvent(event);
}
