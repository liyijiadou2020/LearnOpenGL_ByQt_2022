#include "lyj_widget.h"


LYJ_Widget::LYJ_Widget(QWidget *parent) : QOpenGLWidget(parent)
{
    // 1. QTimer连接
    connect(&tm_,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    // 2. QTimer启动
    tm_.start(60);
}

LYJ_Widget::~LYJ_Widget()
{

}

void LYJ_Widget::initializeGL()
{
    // 0. 函数指针实例化
    initializeOpenGLFunctions();

    // 1. 花托渲染器实例化
    cubeRender_.initsize(0.5);
    // 2. 设置相机的观察点
    camera_.setX(3);
    camera_.setY(0);
    camera_.setZ(3);
}

void LYJ_Widget::resizeGL(int w, int h)
{
    pMatrix_.setToIdentity();
    pMatrix_.perspective(45,float(w)/h,0.01f,100.0f);
}

void LYJ_Widget::paintGL()
{
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glClearColor(0.2f,0.3f,0.3f,1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    vMatrix.lookAt(camera_, QVector3D(0.0,0.0,0.0), QVector3D(0.0,1.0,0.0));

    QMatrix4x4 mMatrix;
    mMatrix.rotate(angleX_,1,0,0);
    mMatrix.rotate(angleY_,0,1,0);
    mMatrix.rotate(angleZ_,0,0,1);
    cubeRender_.render(f, pMatrix_, vMatrix, mMatrix);
}

void LYJ_Widget::slotTimeout()
{
    angleX_ += 5;
//    angleY_ += 5;
//    angleZ_ += 5;
    update();
}


