// 构造函数
AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // 图形初始化
    // 摄像机初始化
    // 键鼠跟踪设置
    // 时钟和计时器初始化

}    

AXBOpenGLWidget::~AXBOpenGLWidget()
{
    if (!isValid()) return;
    makeCurrent();
    // 回收对象

    update();
}



void AXBOpenGLWidget::initializeGL()
{
    // 1. 实例化函数指针
    initializeOpenGLFunctions();

    // 2. 创建VBO，VAO，EBO对象，并赋予ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 3. 配置球体
    // 3.1 编译链接着色器程序，绑定程序后设置uniform值
    // 3.2 【VAO, VBO】绑定VAO, VBO对象，为VBO对象创建数据存储
    // 3.3 【VAO】告知显卡如何解析缓冲里的属性值，开启属性值
    // 3.4 【EBO】EBO绑定，传入为EBO对象创建数据存储
    // 3.5 模型矩阵位移缩放
    // 3.6 解绑VAO, VBO, EBO


    // 4. 椎体着色器程序
    // 4.1 编译链接着色器程序，绑定程序后设置uniform值
    // 4.2 【VAO, VBO】绑定VAO, VBO对象，为VBO对象创建数据存储
    // 4.3 【VAO】告知显卡如何解析缓冲里的属性值，开启属性值
    // 4.4 【EBO】EBO绑定，传入为EBO对象创建数据存储
    // 4.5 模型矩阵位移缩放
    // 4.6 解绑VAO, VBO, EBO


    // 5. 立方体着色器程序
    // 5.1 编译链接着色器程序，绑定程序后设置uniform值
    // 5.2 【VAO, VBO】绑定VAO, VBO对象，为VBO对象创建数据存储
    // 5.3 【VAO】告知显卡如何解析缓冲里的属性值，开启属性值
    // 5.4 【EBO】EBO绑定，传入为EBO对象创建数据存储
    // 5.5 模型矩阵位移缩放
    // 5.6 解绑VAO, VBO, EBO

}


void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}


void AXBOpenGLWidget::paintGL()
{
    // 1. 设置视口，设置背景颜色，设置深度测试

    // 2. 设置view矩阵和projection矩阵

    // 3. 判断当前要画的图形
    // 3.1 画球体
    // 3.1.1 绑定VAO
    // 3.1.2 绑定着色器程序
    // 3.1.3 设置矩阵：view, projection, model
    // 3.1.4 EBO绘制
    // 3.1.5 解绑VAO
    
    // 3.2 画球体
    // 3.2.1 绑定VAO
    // 3.2.2 绑定着色器程序
    // 3.2.3 设置矩阵：view, projection, model
    // 3.2.4 EBO绘制
    // 3.2.5 解绑VAO


    // 3.3 画立方体
    // 3.3.1 绑定VAO
    // 3.3.2 绑定着色器程序
    // 3.3.3 设置矩阵：view, projection, model
    // 3.3.4 EBO绘制
    // 3.3.5 解绑VAO
}


void AXBOpenGLWidget::drawShape(AXBOpenGLWidget::Shape shape)
{
    m_shape=shape;
    // 这条语句会重新调用paintGL()
    update();
}

void AXBOpenGLWidget::setWirefame(bool wireframe)
{
    makeCurrent();
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);    
    doneCurrent();
    update();
}

void move3DShape(Qvector3D step)
{
    // 移动球体，如果按键被按下
    // 移动椎体，如果按键被按下
    // 移动立方体，如果按键被按下
}

void AXBOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float deltaTime=TIMEOUTMSEC/1000.0;

    switch (event->key()) {
        case Qt::Key_Up:ratio+=0.1;break;
        case Qt::Key_Down:ratio-=0.1;break;
        case Qt::Key_W: m_camera.ProcessKeyboard(FORWARD,deltaTime);break;
        case Qt::Key_S: m_camera.ProcessKeyboard(BACKWARD,deltaTime);break;
        case Qt::Key_D: m_camera.ProcessKeyboard(RIGHT,deltaTime);break;
        case Qt::Key_A: m_camera.ProcessKeyboard(LEFT,deltaTime);break;

        // 移动图形    
        case Qt::Key_Up:    move3DShape(QVector3D( 0.0f,      moveSpeed, 0.0f));  break;
        case Qt::Key_Down:  move3DShape(QVector3D( 0.0f,     -moveSpeed, 0.0f));  break;
        case Qt::Key_Left:  move3DShape(QVector3D(-moveSpeed, 0.0f,      0.0f));  break;
        case Qt::Key_Right: move3DShape(QVector3D( moveSpeed, 0.0f,      0.0f));  break;

        default:
            break;
    }
    if(ratio>1) ratio=1;
    if(ratio<0) ratio=0;

    // 设置透明度
    makeCurrent();
    _sp_sphere.bind();
    _sp_sphere.setUniformValue("val_alpha", val_alpha);

    _sp_cube.bind();
    _sp_cube.setUniformValue("val_alpha", val_alpha);
    qDebug() << "[INFO] val_alpha=" << val_alpha;
    doneCurrent();
    update();

}


/* 处理鼠标移动事件 */
void FoxOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint last_pos(width()/2, height()/2);
    auto current_pos = event->pos();
    delta_pos = current_pos - last_pos;
    last_pos = current_pos;

    camera_.changeCameraFront(delta_pos.x(), delta_pos.y(), true);

    update();
}


/* 处理鼠标滚轮事件 */
void FoxOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    camera_.changeCameraZoomFov(event->angleDelta().y()/120);  // 一步是 120

    update();
}


void FoxOpenGLWidget::updateGL()
{
    update();
}

