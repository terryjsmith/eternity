#ifndef ETERNITYOPENGLWIDGET_H
#define ETERNITYOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTime>
#include <QSurface>

#include <eternity.h>
#include <Render/View.h>

class EternityOpenGLWidget : public QOpenGLWidget
{
public:
    EternityOpenGLWidget(QWidget *parent = 0);

    void initializeGL() override;
    void resizeGL(int w, int h)override;
    void paintGL()override;

protected:
    int m_framebufferWidth, m_framebufferHeight;

    // Current scene
    View* m_scene;

    // Our update time (for retrieving frame delta)
    QTime* m_updateTimer;

    bool m_initialized;
};

#endif // ETERNITYOPENGLWIDGET_H
