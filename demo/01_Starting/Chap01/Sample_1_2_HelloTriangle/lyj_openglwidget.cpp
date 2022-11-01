#include "lyj_openglwidget.h"

// Chap01 - Hello Triangle
//创建VBO和VAO对象，并赋予ID。 绑定VBO和VAO对象
unsigned int VBO, VAO;
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

LYJ_OpenGLWidget::LYJ_OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

LYJ_OpenGLWidget::~LYJ_OpenGLWidget()
{
    makeCurrent();
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
    doneCurrent();
}


void LYJ_OpenGLWidget::drawShape(LYJ_OpenGLWidget::Shape shape)
{
    m_shape=shape;
    update();
}

void LYJ_OpenGLWidget::setWirefame(bool wireframe)
{
    makeCurrent();
    if(wireframe) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    update();
    doneCurrent();
}

void LYJ_OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Chap01 - Hello Triangle
    // 创建VBO和VAO对象，并赋予ID。 【绑定VBO和VAO对象】
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO需要指定类型。绑定之后这个VBO就会记录GL_ARRAY_BUFFER的状态了

    //【VBO管理】为当前绑定到target的缓冲区对象创建一个新的数据存储。
    //如果data不是NULL，则使用来自此指针的数据初始化数据存储
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //【VAO管理】告知显卡如何解析缓冲里的第0个属性值 & 开启VAO管理的第0个属性值
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // VBO休息
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 链接和编译着色器
    bool success;
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");
    success=shaderProgram.link();
    if(!success)
        qDebug()<<"ERROR:"<<shaderProgram.log();

    // 【VAO最后休息！！】
    glBindVertexArray(0);
}

void LYJ_OpenGLWidget::resizeGL(int w, int h)
{

}

void LYJ_OpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绑定编译好的着色器程序
    shaderProgram.bind();

    // 先绑定【VAO】再进行绘制
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制

} // paintGL
