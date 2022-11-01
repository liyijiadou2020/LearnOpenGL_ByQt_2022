#ifndef CUBERENDER_H
#define CUBERENDER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>

class CubeRender
{

public:
    CubeRender() = default;
    void initsize(float length);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix);

private:
    QOpenGLShaderProgram program_;
    QOpenGLBuffer vbo_;
    QVector<GLfloat> vertPoints_;
    QVector<QVector3D> cubePositions;
};

#endif // CUBERENDER_H
