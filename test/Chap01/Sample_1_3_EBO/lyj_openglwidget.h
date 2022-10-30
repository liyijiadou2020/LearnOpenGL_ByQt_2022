#ifndef LYJ_OPENGLWIDGET_H
#define LYJ_OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
// 着色器程序
#include <QOpenGLShaderProgram>

class LYJ_OpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{None,Rect,Circle,Triangle};
    explicit LYJ_OpenGLWidget(QWidget *parent = nullptr);
    ~LYJ_OpenGLWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();


signals:
private:
    Shape m_shape; //要绘制的图形
    QOpenGLShaderProgram shaderProgram;
};

#endif // LYJ_OPENGLWIDGET_H
