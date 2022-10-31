#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
#include "torusrender.h"


class Widget : public QOpenGLWidget
{
    // 在声明的最顶部使用 Q_OBJECT，这样才能使用槽和信号
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // QTimer是一个定时器，设定定时器可以定时调用函数
    QTimer tm_;
    TorusRender render_;
    QMatrix4x4 pMatrix_;

    // 和x轴的夹角？
    qreal angleX_ = 0,angleY_ = 0,angleZ_ = 0;

    QVector3D camera_;

    // slot：槽函数
private slots:
    void slotTimeout();
};

#endif // WIDGET_H
