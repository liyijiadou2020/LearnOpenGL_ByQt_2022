#pragma once
#include <QOpenGLWidget>
#include <qopenglfunctions_4_3_core.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class MyOpenglWedgit : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    float cameraX, cameraY, cameraZ;
    float cubeLocX, cubeLocY, cubeLocZ;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vao;
    GLuint mvLoc, projLoc;
    float aspect;
    QMatrix4x4 pMat, vMat, mMat, mvMat;
    void setupVertices();
    MyOpenglWedgit(QWidget*parent);
    ~MyOpenglWedgit();
protected:
    void initializeGL();
    void resizeGL();
    void paintGL();
};
