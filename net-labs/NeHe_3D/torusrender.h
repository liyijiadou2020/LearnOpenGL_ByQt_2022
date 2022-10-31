#ifndef TORUSRENDER_H
#define TORUSRENDER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QTime>

#define PI 3.14159265f
class TorusRender : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
public:
    TorusRender(QWidget *parent = nullptr);
    void initsize(float rBig,float rSmall,int nCol,int nRow,QImage &img);
    void initsize(float rBig,float rSmall,int nCol,int nRow);
    void render(QOpenGLExtraFunctions *f,QMatrix4x4 &pMatrix,QMatrix4x4 &vMatrix,QMatrix4x4 &mMatrix);

private:
    QOpenGLShaderProgram program_;
    QOpenGLBuffer vbo_;
    QVector<GLfloat> vertPoints_,textPoints_;
    QOpenGLTexture *texture_{nullptr};
    bool userTexture_ = false;
};

#endif // TORUSRENDER_H
