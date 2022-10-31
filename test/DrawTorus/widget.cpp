#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    /* QTimer的使用流程
    首先先创建一个QTimer对象–>
    通过QTimer中的start方法让它开始计时
    （start方法可以设定定时运行的时间）–>
    每当计时的时间超过了给定的时间后，就会调用一次timeout.connect(xx)中的xx函数–>
    使用完后调用stop方法关闭计时器
    */
    // 这个connet的意思是，每次计时器的timeout被触发时发出信号，去调用槽函数slotTimeout()
    connect(&tm_, SIGNAL(timeout()),this,SLOT(slotTimeout()));
    tm_.start(60);
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    // 初始化得到一个花托渲染器
    render_.initsize(1.2, 0.5, 13, 30);
    // 设置相机的观察点
    camera_.setX(3);
    camera_.setY(0);
    camera_.setZ(3);
}

// resizeGL()函数就是设置窗体的，比如窗体大小、比例、投影方式（正投影还是透视投影）
// 应该在第一次调用paintGL()之前，但在initializeGL()之后调用resizeGL()函数。
// 在窗口部件改变大小时也将调用resizeGL()函数
void Widget::resizeGL(int w, int h)
{
    // QMatrix4x4类可以表示一个3D空间中的4X4变换矩阵，
    //perspective()函数用来设置透视投影矩阵，这里设置了视角为45°，纵横比为窗口的纵横比，
    //    最近的位置为0.1，最远的位置为100.
    pMatrix_.setToIdentity();
    // 视角45度，纵横比是窗口的纵横比，
    pMatrix_.perspective(45, float(w)/h, 0.01f, 100.0f);
}

void Widget::paintGL()
{
    // #TODO 这个f是用来干啥的？？？
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
    f->glClearColor(0.2f,0.3f,0.3f,1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 【视角矩阵：摄像机不动】
    QMatrix4x4 vMatrix;
    vMatrix.lookAt(camera_, QVector3D(0.0,0.0,0.0), QVector3D(0.0,1.0,0.0));

    // 【变换矩阵：完成旋转】
    QMatrix4x4 mMatrix;
    mMatrix.rotate(angleX_,1,0,0);
    mMatrix.rotate(angleY_,0,1,0);
    mMatrix.rotate(angleZ_,0,0,1);

    render_.render(f,pMatrix_,vMatrix,mMatrix);
}

// X角+5， Y角+5， Z角+5
void Widget::slotTimeout()
{
    angleX_ += 5;
//    angleY_ += 5;
//    angleZ_ += 5;

    update(); // 在这里触发paintGL()
}
