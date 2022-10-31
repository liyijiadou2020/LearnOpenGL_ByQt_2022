#ifndef TORUSRENDER_H
#define TORUSRENDER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>

#define PI 3.14159265f

class TorusRender
{
public:
    TorusRender() = default;
    void initsize(float rBig,float rSmall,int nCol,int nRow,QImage &img);
    void initsize(float rBig,float rSmall,int nCol,int nRow);
    void render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix);

private:
    QOpenGLShaderProgram program_;
    QOpenGLBuffer vbo_;
    QVector<GLfloat> vertPoints_;

    // #TODO 纹理，尝试丢弃
    QVector<GLfloat> textPoints_;
    QOpenGLTexture *texture_{nullptr};
    bool userTexture_ = false;
};

#endif // TORUSRENDER_H
