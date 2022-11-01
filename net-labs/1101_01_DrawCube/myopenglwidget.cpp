//#include "stdafx.h"
#include "myopenglwidget.h"

MyOpenglWedgit::MyOpenglWedgit(QWidget*parent)
    : QOpenGLWidget(parent)
{

}

MyOpenglWedgit::~MyOpenglWedgit()
{

}
void MyOpenglWedgit::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);//设置深度缓存
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.2, 0.3, 0.3, 0.5);//改变窗口的背景颜色

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vs")) {     //添加并编译顶点着色器
        qDebug() << "编译ERROR:" << shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.fs")) {   //添加并编译片段着色器
        qDebug() << "编译ERROR:" << shaderProgram.log();    //如果编译出错,打印报错信息
    }

    if (!shaderProgram.link()) {                      //链接着色器
        qDebug() << "链接ERROR:" << shaderProgram.log();    //如果链接出错,打印报错信息
    }
    shaderProgram.bind();
    cameraX = 0.0f; cameraY = 1.0f; cameraZ = 10.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = -0.0f;
    setupVertices();
    mvLoc = shaderProgram.uniformLocation("mv_matrix");
    projLoc = shaderProgram.uniformLocation("proj_matrix");//获取程序的统一变量mv，投影矩阵
    //构建透视矩阵
    aspect = (float)width() / (float)height();
    pMat.perspective(70.0472f, aspect, 0.1f, 1000.f);
    //构建视图矩阵
    vMat.translate(-cameraX, -cameraY, -cameraZ);
    mMat.translate(cubeLocX, cubeLocY, cubeLocZ);
    mvMat = vMat * mMat;
    //将mv矩阵发送给对应的统一变量
    shaderProgram.setUniformValue(mvLoc, mvMat);
    shaderProgram.setUniformValue(projLoc, pMat);
}

void MyOpenglWedgit::resizeGL()
{
    glViewport(0, 0, width(), height());
}

void MyOpenglWedgit::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLuint vPosition = shaderProgram.attributeLocation("vPosition");
    shaderProgram.setAttributeBuffer(vPosition, GL_FLOAT,0,3,0);
    glEnableVertexAttribArray(vPosition);
    //调整OpenGL设置，绘制模型
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);//线框模式
    glDrawArrays(GL_TRIANGLES,0,36);
    MyOpenglWedgit::update();
}
void MyOpenglWedgit::setupVertices() {
    float vertexPositions[108] = {
        -1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
       1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,-1.0f,
        1.0f,-1.0f,1.0f,1.0f, 1.0f, 1.0f,1.0f,1.0f,-1.0f,
         1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,
         -1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,
        -1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f
    };
    vao.create();
    vao.bind();
    vao.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vao.allocate(vertexPositions, sizeof(vertexPositions));
}
