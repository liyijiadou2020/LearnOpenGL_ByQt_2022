#ifndef OPENGLWIDGET02_H
#define OPENGLWIDGET02_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class OpenGLWidget02 : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget02(QWidget* parent);
    ~OpenGLWidget02();
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
};

#endif // OPENGLWIDGET02_H
