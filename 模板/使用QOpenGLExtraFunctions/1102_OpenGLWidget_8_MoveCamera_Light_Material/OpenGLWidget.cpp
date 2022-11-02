#include "OpenGLWidget.h"
#include <QDebug>
#include <QOpenGLDebugLogger>
#include <limits>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

#include "LightModel.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
    //    我们在3D窗口初始化的时候，通过格式设置多重采样
	auto newFormat = this->format();
	newFormat.setSamples(16);
	this->setFormat(newFormat);

	startTimer(1000 / 60);
    /* 摄像机 */
	m_camera.move(-6, 0, 3);
	m_camera.look(0, 30, 0);
	m_camera.update();
    /* 光源 */
	m_light.setPos({ 10, 3 , 0 });
	m_light.setColor(QColor(255, 255, 255));

	installEventFilter(&m_camera);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
    //glClearColor(0, 0.5, 0.7, 1);
	//auto _color = m_light.color() * 0.1;
	//glClearColor(_color.x(), _color.y(), _color.z(), 1);
	glClearColor(0, 0, 0, 1);

    //    然后为所有的模型添加必要的信息：
	for (int i = 0; i < 3; ++i)
	{
		auto _dice = new Dice();
        // 摄像机，光源，位置
		_dice->setCamera(&m_camera);
		_dice->setLight(&m_light);
		_dice->setPos({ 0, i * 3.f, 0 });

		_dice->init();

		m_models << _dice;
	}

	m_lightModel = new LightModel();
	m_lightModel->setCamera(&m_camera);
	m_lightModel->setLight(&m_light);
	m_lightModel->setPos(m_light.pos());
	//m_lightModel->setScale(0.1);
	m_lightModel->init();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    // 给模型设置投影矩阵
	m_projection.setToIdentity();
	m_projection.perspective(60, (float)w / h, 0.001, 1000);
	for (auto dice : m_models)
	{
		dice->setProjection(m_projection);
	}
	m_lightModel->setProjection(m_projection);
}

void OpenGLWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_CULL_FACE); // 为了节省资源，我们可以开启背面裁剪
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (auto dice : m_models)
	{
		dice->paint();
	}

//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_lightModel->paint();

    /* 绘制表示摄像机的瞄准十字 */
    // 在Qt的窗口中绘制UI,记得在QPainter构建之前加上两句话禁用深度测试和背面裁剪
	QPainter _painter(this);
    _generateCenterCross(_painter);
}

void OpenGLWidget::_generateCenterCross(QPainter& _painter){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

void OpenGLWidget::timerEvent(QTimerEvent *event)
{
	m_camera.update();
	float _speed = 0.1;
	for (auto dice : m_models)
	{
        float _y = dice->rotate().y() + _speed; // 绕y轴旋转
		if (_y >= 360)
			_y -= 360;
        dice->setRotate({ 0, _y, 0 });
        _speed += 1.0; // 每个色子的速度是不同的
	}

    //    光污染：这里每帧修改一次光的颜色，但是没有必要
	//auto _h = m_light.color().hsvHue() + 1;
	//if (_h >= 360)
	//	_h -= 360;
	//m_light.setColor(QColor::fromHsv(_h, 255, 255));

	m_lightModel->setPos(m_light.pos());
	repaint();
}
