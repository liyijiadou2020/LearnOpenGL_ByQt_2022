#ifndef AXBOPENGLWIDGET_H
#define AXBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QTime>
#include "camera.h"

class AXBOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{None,Rect,Circle,Triangle};
    explicit AXBOpenGLWidget(QWidget *parent = nullptr);
    ~AXBOpenGLWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void keyPressEvent(QKeyEvent *event);
    // 【摄像机】鼠标移动改变视觉
    void mouseMoveEvent(QMouseEvent* event);
    // 【摄像机】鼠标滚轮改变视觉大小
    void wheelEvent(QWheelEvent* event);


signals:

public slots:
    void on_timeout();
private:
    Shape m_shape;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLTexture * textureWall;
    QOpenGLTexture * textureSmile;
    QOpenGLTexture * textureSmall;
    QTimer timer;
    QTime m_time;
    Camera m_camera;

    // 摄像机

//    QVector3D cameraPos;
//    QVector3D cameraTarget;
//    QVector3D cameraDirection;
//    QVector3D up;
//    QVector3D cameraUp;
//    QVector3D cameraRight;
//    QVector3D cameraFront;

};



#endif // AXBOPENGLWIDGET_H
