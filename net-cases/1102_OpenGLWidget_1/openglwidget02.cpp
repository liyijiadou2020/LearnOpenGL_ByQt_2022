#include "openglwidget02.h"

OpenGLWidget02::OpenGLWidget02(QWidget* parent)
    : QOpenGLWidget(parent)
{

}

OpenGLWidget02::~OpenGLWidget02()
{

}

void OpenGLWidget02::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0.5, 0.7, 1);


}

void OpenGLWidget02::paintGL()
{

}

void OpenGLWidget02::resizeGL(int w, int h)
{

}
