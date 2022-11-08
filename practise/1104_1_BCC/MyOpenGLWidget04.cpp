#include "MyOpenGLWidget04.h"
#include <QDebug>
#include <QOpenGLDebugLogger>
#include <limits>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <math.h>
#include <iostream>

#include "LightModel.h"
#include "BallModel.h"
#include "ConeModel.h"
#include "CubeModel.h"

#define TIMEOUT 50  // 50 �������һ��

unsigned long long gl_time = 0;

MyOpenGLWidget04::MyOpenGLWidget04(QWidget *parent)
    : QOpenGLWidget(parent)
{
    //    ������3D���ڳ�ʼ����ʱ��ͨ����ʽ���ö��ز���
    //	auto newFormat = this->format();
    //	newFormat.setSamples(16);
    //    this->setFormat(newFormat);

    startTimer(1000 / 60);
    /* ����� */
    m_camera.move(-6, 0, 3);
    m_camera.look(0, 30, 0);
    m_camera.update();
    /* ��Դ */
    m_light.setPos({ -5, 3, 3 });
    m_light.setColor(QColor(255, 255, 255));
    /* ���������ؼ��̵�WASDC��Space */
    installEventFilter(&m_camera);
    /* ʱ�� */
    m_time.start();
}

MyOpenGLWidget04::~MyOpenGLWidget04()
{
}

void MyOpenGLWidget04::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.2, 0.3, 0.3, 1);
//    glClearColor(0.4f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (true) {
        auto _dice = new CubeModel();
        // ���������Դ��λ��
        _dice->setCamera(&m_camera);
        _dice->setLight(&m_light);
        _dice->setPos({ 0, 0, 0 });
        _dice->init();
        m_models << _dice;
    }

//    // ��һ��Բ׶
//    if (true) {
//        auto _c = new ConeModel();
//        _c->setCamera(&m_camera);
//        _c->setLight(&m_light);
//        _c->setPos({ -4, 1, 0 });
//        _c->init();
//        _c->setScale(3.0);
//        m_models << _c;
//    }

//    // ��һ����
//    if (true) {
//        auto _b = new BallModel();
//        // ���������Դ��λ��
//        _b->setCamera(&m_camera);
//        _b->setLight(&m_light);
//        _b->setPos({ 0, 3, 0 });
//        _b->init();
//        m_models << _b;
//    }

    // ��Դ
    m_lightModel = new LightModel();
    m_lightModel->setCamera(&m_camera);
    m_lightModel->setLight(&m_light);
    m_lightModel->setPos(m_light.pos());
    m_lightModel->setScale(0.2);
    m_lightModel->init();
}

void MyOpenGLWidget04::resizeGL(int w, int h)
{
    // ��ÿ��ģ������ͶӰ����
    m_projection.setToIdentity();
    m_projection.perspective(60, (float)w / h, 0.001, 1000);
    for (auto dice : m_models)
    {
        dice->setProjection(m_projection);
    }
    m_lightModel->setProjection(m_projection);
}

void MyOpenGLWidget04::paintGL()
{
    glEnable(GL_DEPTH_TEST); // ������Ȳ���
    //    glEnable(GL_CULL_FACE); // Ϊ�˽�ʡ��Դ�����ǿ��Կ�������ü�
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (auto dice : m_models)
    {
        dice->paint();
    }

    // �õƹ���������ת
    float time=m_time.elapsed()/50.0;
    m_light.setX(cos(time/50)*5.0);
    m_light.setZ(sin(time/50)*5.0);

    m_lightModel->paint();

    /* ���Ʊ�ʾ���������׼ʮ�� */
    // ��Qt�Ĵ����л���UI,�ǵ���QPainter����֮ǰ�������仰������Ȳ��Ժͱ���ü�
    //	QPainter _painter(this);
    //  _generateCenterCross(_painter);
}

void MyOpenGLWidget04::_generateCenterCross(QPainter& _painter){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    auto _rect = this->rect();
    _painter.setPen(Qt::green);
    _painter.drawLine(_rect.center() + QPoint{ 0, 5 }, _rect.center() + QPoint{ 0, 15 });
    _painter.drawLine(_rect.center() + QPoint{ 0, -5 }, _rect.center() + QPoint{ 0, -15 });
    _painter.drawLine(_rect.center() + QPoint{ 5, 0 }, _rect.center() + QPoint{ 15, 0 });
    _painter.drawLine(_rect.center() + QPoint{ -5, 0 }, _rect.center() + QPoint{ -15, 0 });

    _painter.drawText(QPoint{ 5, 15 }, QString(u8"Camera Position: (%1, %2, %3)")
                      .arg(m_camera.pos().x(), 0, 'f', 3).arg(m_camera.pos().y(), 0, 'f', 3).arg(m_camera.pos().z(), 0, 'f', 3));
    _painter.drawText(QPoint{ 5, 30 }, QString(u8"Camera Angle: (%1, %2, %3)")
                      .arg(m_camera.yaw(), 0, 'f', 3).arg(m_camera.pitch(), 0, 'f', 3).arg(m_camera.roll(), 0, 'f', 3));
}

void MyOpenGLWidget04::move3DShape(QVector3D step)
{
    if (is_move_dice)
    {
        m_models[0]->move(step);
    }

    if (is_move_ball)
    {
        m_models[2]->move(step);
    }

    if (is_move_cone)
    {
        m_models[1]->move(step);
    }
}

void MyOpenGLWidget04::changeColorOfLight()
{
    /* �ı���ɫ */
        if (true) {
            // ʹ��hsvģ�ͣ��Ϳ���ͨ����һ���������������е���ɫ��
            auto _h = m_light.color().hsvHue() + 1;
            if (_h >= 360)
                _h -= 360;
            m_light.setColor(QColor::fromHsv(_h, 255, 255));
        }
}

void MyOpenGLWidget04::updateGL()
{
    update();
}

void MyOpenGLWidget04::timerEvent(QTimerEvent *event)
{
    m_camera.update();
    float _speed = 0.1;
    for (auto dice : m_models)
    {
        float _y = dice->rotate().y() + _speed; // ��y����ת
        if (_y >= 360)
            _y -= 360;
        dice->setRotate({ 0, _y, 0 });
        _speed += 1.0; // ÿ��ģ�͵�ת���ٶ��ǲ�ͬ��
    }

    m_lightModel->setPos(m_light.pos());

    /* �ı���ɫ */
    //    if (true) {
    //        // ʹ��hsvģ�ͣ��Ϳ���ͨ����һ���������������е���ɫ��
    //        auto _h = m_light.color().hsvHue() + 1;
    //        if (_h >= 360)
    //            _h -= 360;
    //        m_light.setColor(QColor::fromHsv(_h, 255, 255));
    //    }

    //    repaint();
    update();
}


void MyOpenGLWidget04::keyPressEvent(QKeyEvent *event)
{

    float moveSpeed = 0.05;
    switch (event->key())
    {
    case Qt::Key_Up:    move3DShape(QVector3D( 0.0f,      moveSpeed, 0.0f));  break;
    case Qt::Key_Down:  move3DShape(QVector3D( 0.0f,     -moveSpeed, 0.0f));  break;
    case Qt::Key_Left:  move3DShape(QVector3D(-moveSpeed, 0.0f,      0.0f));  break;
    case Qt::Key_Right: move3DShape(QVector3D( moveSpeed, 0.0f,      0.0f));  break;
    case Qt::Key_0:     changeColorOfLight();                                 break;
    case Qt::Key_1:     m_light.setColor(QColor(255, 255, 255));              break;
    default: break;
    }
    update();
}






