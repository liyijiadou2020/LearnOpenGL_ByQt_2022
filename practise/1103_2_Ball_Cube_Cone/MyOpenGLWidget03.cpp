#include "MyOpenGLWidget03.h"
#include <QDebug>
#include <QOpenGLDebugLogger>
#include <limits>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

#include "StableLightModel.h"

#define TIMEOUT 50  // 50 �������һ��

MyOpenGLWidget03::MyOpenGLWidget03(QWidget *parent)
	: QOpenGLWidget(parent)
{
    //    ������3D���ڳ�ʼ����ʱ��ͨ����ʽ���ö��ز���
	auto newFormat = this->format();
	newFormat.setSamples(16);
	this->setFormat(newFormat);

	startTimer(1000 / 60);
    /* ����� */
	m_camera.move(-6, 0, 3);
	m_camera.look(0, 30, 0);
	m_camera.update();
    /* ��Դ */
	m_light.setPos({ 10, 3 , 0 });
    m_light.setColor(QColor(255, 255, 255));

	installEventFilter(&m_camera);
}

MyOpenGLWidget03::~MyOpenGLWidget03()
{
}

void MyOpenGLWidget03::initializeGL()
{
	initializeOpenGLFunctions();

    glClearColor(0.2, 0.3, 0.3, 1);

    //    Ȼ��Ϊ���е�ģ����ӱ�Ҫ����Ϣ��
    for (int i = 0; i < 4; ++i)
    {
        auto _dice = new CubeModel();
        // ���������Դ��λ��
        _dice->setCamera(&m_camera);
        _dice->setLight(&m_light);
        _dice->setPos({ 0, i * 3.f, 0 });

        _dice->init();

        m_models << _dice;
    }

    // ��һ����
    if (true) {
        auto _b = new StableLightModel();
        // ���������Դ��λ��
        _b->setCamera(&m_camera);
        _b->setLight(&m_light);
        _b->setPos({ 0, 5, -10 });

        _b->init();

        m_models << _b;
    }

    // ��Դ
    m_lightModel = new StableLightModel();
	m_lightModel->setCamera(&m_camera);
	m_lightModel->setLight(&m_light);
	m_lightModel->setPos(m_light.pos());
	//m_lightModel->setScale(0.1);
	m_lightModel->init();
}

void MyOpenGLWidget03::resizeGL(int w, int h)
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

void MyOpenGLWidget03::paintGL()
{
    glEnable(GL_DEPTH_TEST); // ������Ȳ���
    glEnable(GL_CULL_FACE); // Ϊ�˽�ʡ��Դ�����ǿ��Կ�������ü�
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (auto dice : m_models)
	{
		dice->paint();
	}


//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_lightModel->paint();


    /* ���Ʊ�ʾ���������׼ʮ�� */
    // ��Qt�Ĵ����л���UI,�ǵ���QPainter����֮ǰ�������仰������Ȳ��Ժͱ���ü�
	QPainter _painter(this);
    _generateCenterCross(_painter);
}

void MyOpenGLWidget03::_generateCenterCross(QPainter& _painter){
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

void MyOpenGLWidget03::move3DShape(QVector3D step)
{
    if (is_move_sphere)
    {
//        _sphere.mat_model.translate(step);
    }

    if (is_move_cone)
    {
//        _cone.mat_model.translate(step);
    }

    if (is_move_cube)
    {
//        _cube.mat_model.translate(step);
    }
}

void MyOpenGLWidget03::updateGL()
{
    update();
}

void MyOpenGLWidget03::timerEvent(QTimerEvent *event)
{
	m_camera.update();
	float _speed = 0.1;
	for (auto dice : m_models)
	{
        float _y = dice->rotate().y() + _speed; // ��y����ת
		if (_y >= 360)
			_y -= 360;
        dice->setRotate({ 0, _y, 0 });
        _speed += 1.0; // ÿ��ɫ�ӵ��ٶ��ǲ�ͬ��
	}

	m_lightModel->setPos(m_light.pos());
    repaint();
}


void MyOpenGLWidget03::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = (float)TIMEOUT / (float)1000;
    float moveSpeed = 0.05;

    switch (event->key())
    {
        case Qt::Key_Up:    move3DShape(QVector3D( 0.0f,      moveSpeed, 0.0f));  break;
        case Qt::Key_Down:  move3DShape(QVector3D( 0.0f,     -moveSpeed, 0.0f));  break;
        case Qt::Key_Left:  move3DShape(QVector3D(-moveSpeed, 0.0f,      0.0f));  break;
        case Qt::Key_Right: move3DShape(QVector3D( moveSpeed, 0.0f,      0.0f));  break;

        default: break;
    }

    update();
}






