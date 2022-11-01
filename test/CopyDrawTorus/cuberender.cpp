#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <QOpenGLWidget>  // 相当于GLFW
#include <QOpenGLFunctions_3_3_Core>  // 相当于 GLAD
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "cuberender.h"
#include <iostream>

unsigned int indices[] = { // note that we start from 0!
                           0, 1, 3, // first triangle
                           1, 2, 3 // second triangle
                         };


void CubeRender::initsize(float r)
{
    // 1. 编译链接着色器
    program_.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,":/shapes.vert");
    program_.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,":/shapes.frag");
    program_.link();

    // 2. 生成顶点坐标
    this->vertPoints_ = {
        // 顶点               // 颜色
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    this->cubePositions= {
        QVector3D( 0.0f, 0.0f, 0.0f),
        QVector3D( 2.0f, 5.0f, -15.0f),
        QVector3D(-1.5f, -2.2f, -2.5f),
        QVector3D(-3.8f, -2.0f, -12.3f),
        QVector3D( 2.4f, -0.4f, -3.5f),
        QVector3D(-1.7f, 3.0f, -7.5f),
        QVector3D( 1.3f, -2.0f, -2.5f),
        QVector3D( 1.5f, 2.0f, -2.5f),
        QVector3D( 1.5f, 0.2f, -1.5f),
        QVector3D(-1.3f, 1.0f, -1.5f)
    };

    // 3. 创建VBO对象，绑定VBO对象，向VBO的缓冲区传入顶点
    vbo_.create();
    vbo_.bind();
    // allocate(x, y)-向缓冲区申请y字节的空间，使用data的内容来初始化。
    vbo_.allocate(vertPoints_.data(), vertPoints_.count() * sizeof(GLfloat));
}

void CubeRender::render(QOpenGLExtraFunctions *f, QMatrix4x4 &pMatrix, QMatrix4x4 &vMatrix, QMatrix4x4 &mMatrix)
{
    // 1. 使用f打开深度测试和GL_CULL_FACE
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);

    // 2. 设置着色器
    //  2.1. 绑定着色器程序和VBO对象
    program_.bind();
    vbo_.bind();

    //  2.2. 设置着色器的3个矩阵
    program_.setUniformValue("uPMatrix",pMatrix);
    program_.setUniformValue("uVMatrix",vMatrix);
    program_.setUniformValue("uMMatrix",mMatrix);

    //  2.3. 开启着色器的属性0：顶点坐标   
    program_.enableAttributeArray(0);
    program_.enableAttributeArray(1);
    //program_.setAttributeBuffer(0,GL_FLOAT,0,3,3*sizeof(GLfloat));
    program_.setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof (GLfloat));
    program_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof (GLfloat), 2, 5 * sizeof (GLfloat));


    //  2.4. 使用f来画出VBO缓冲区的三角形(在这里指定了线框渲染)
    f->glDrawArrays(GL_LINE_STRIP, 0, vertPoints_.count() / 5);

    //  2.5. 关闭属性0
//    program_.disableAttributeArray(0);
//    program_.disableAttributeArray(1);

    // 3. 释放vbo和着色器程序
    vbo_.release();
    program_.release();
    // 4. 使用f关闭GL_CULL_FACE和深度测试
    f->glDisable(GL_CULL_FACE);
    f->glDisable(GL_DEPTH_TEST);

}






