#ifndef LYJ_WIDGET_H
#define LYJ_WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include "cuberender.h"

class LYJ_Widget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    /* 构建和析构函数 */
    explicit LYJ_Widget(QWidget *parent = nullptr);
    ~LYJ_Widget();
    /* */

    /*重载的3个函数*/
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    // 定时器
    QTimer tm_;
    // 渲染器
    CubeRender cubeRender_;
    // 【投影矩阵】
    QMatrix4x4 pMatrix_;
    // 用来设置【变换矩阵】的偏移角
    qreal angleX_ = 0,angleY_ = 0,angleZ_ = 0;
    // 摄像机（一个3D向量而已）
    QVector3D camera_;

    // 记录哪些图形当前绘制
    bool isSelectedCube = false;

private slots:
    void slotTimeout();
};

#endif // LYJ_WIDGET_H
