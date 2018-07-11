#ifndef ETERNITYOPENGLWIDGET_H
#define ETERNITYOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTime>
#include <QSurface>

#include <eternity.h>
#include <Render/Scene.h>

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
    Scene* m_scene;
};

#endif // ETERNITYOPENGLWIDGET_H
