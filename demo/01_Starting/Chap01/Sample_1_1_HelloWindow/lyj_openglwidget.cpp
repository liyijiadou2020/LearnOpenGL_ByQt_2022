#include "lyj_openglwidget.h"

LYJ_OpenGLWidget::LYJ_OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void LYJ_OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

}

void LYJ_OpenGLWidget::resizeGL(int w, int h)
{

}

void LYJ_OpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

}
