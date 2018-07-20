#ifndef ETERNITYOPENGLWIDGET_H
#define ETERNITYOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTime>
#include <QSurface>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <eternity.h>
#include <Render/View.h>

class EternityOpenGLWidget : public QOpenGLWidget
{
public:
    EternityOpenGLWidget(QWidget *parent = 0);

    enum {
        EDITORSTATE_TRANSLATE_X = 1,
        EDITORSTATE_TRANSLATE_Y,
        EDITORSTATE_TRANSLATE_Z,
        EDITORSTATE_TRANSLATE,
    };

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent * event) override;
    void keyReleaseEvent(QKeyEvent * event) override;

protected:
    // The width and height of our window and framebuffer respectively
    int m_width, m_height;
    int m_framebufferWidth, m_framebufferHeight;

    // Current scene
    View* m_scene;

    // Our update time (for retrieving frame delta)
    QTime* m_updateTimer;

    bool m_initialized;

    // The currently selected mesh instance
    MeshComponent* m_currentMesh;
    LightComponent* m_currentLight;

    // The last position of our current mesh before it was moved
    vector3 m_lastPosition;

    // Our current editor state
    int m_editorState;

    // Our last mouse position
    QPoint m_lastMousePos;
    QPoint m_clickMousePos;
    QPoint m_startMovePos;

    // Dragging for camera movement
    bool m_isDragging;
    bool m_isMoving;

    // Our keyboard array
    std::map<int, bool> m_keys;
};

#endif // ETERNITYOPENGLWIDGET_H
