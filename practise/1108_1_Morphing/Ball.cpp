#include "Ball.h"

Ball::Ball()
    :Model()
{
    QVector3D _top{ 0,1,0 };
    float _step = 1;
    QVector<QVector<QVector3D>> _vertexMatrix;

    // �����ǻ��Ʒ�����������Ķ���
    for (float _yaw = 0; _yaw <= 180; _yaw += _step)
    {
        _vertexMatrix << QVector<QVector3D>();
        m_col = 0;
        for (float _pitch = 0; _pitch < 360; _pitch += _step)
        {
            QMatrix4x4 _mat;
            _mat.setToIdentity();
            _mat.rotate(_yaw, 1, 0, 0);
            _mat.rotate(-_pitch, 0, 1, 0);
            auto _p = _top * _mat;
            _vertexMatrix[m_row] << _p;
            ++m_col;
        }
        ++m_row;
    }

    // ������������������Ķ���
    for (int y = 0; y < m_row-1; ++y)
    {
        for (int x = 0; x < m_col; ++x)
        {
            auto _p0 = _vertexMatrix[y][x];
            auto _p1 = _vertexMatrix[y + 1][x];
            int _nextX = x + 1;
            if (_nextX == m_col)
            {
                _nextX = 0;
            }
            auto _p2 = _vertexMatrix[y + 1][_nextX];
            auto _p3 = _vertexMatrix[y][_nextX];
            m_vertices	<< Vertex{ { _p0.x(), _p0.y(), _p0.z() }, {(float)x / m_col,		(float)y / m_row}		, { _p0.x(), _p0.y(), _p0.z() } }
                        << Vertex{ { _p1.x(), _p1.y(), _p1.z() }, {(float)x	/ m_col,		(float)(y + 1) / m_row} , { _p1.x(), _p1.y(), _p1.z() } }
                        << Vertex{ { _p2.x(), _p2.y(), _p2.z() }, {(float)(x + 1) / m_col,	(float)(y + 1) / m_row} , { _p2.x(), _p2.y(), _p2.z() } }
                        << Vertex{ { _p3.x(), _p3.y(), _p3.z() }, {(float)(x + 1) / m_col,	(float)y / m_row}		, { _p3.x(), _p3.y(), _p3.z() } };
        }
    }

    //     ������������������Ļ�
    auto _texture = new QOpenGLTexture(QImage(":/pic/06.png"));
    _texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);
    setTexture(_texture);

    auto _program = new QOpenGLShaderProgram();
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/ball.vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/ball.frag");
//    _program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shader/ball.geom");
    setShaderProgram(_program);
}

Ball::~Ball()
{

}

void Ball::init()
{
    initializeOpenGLFunctions();
    if (!m_vao.isCreated())
        m_vao.create();
    if (!m_vbo.isCreated())
        m_vbo.create();
    if (!m_program->isLinked())
        m_program->link();

    if (m_vertexCount < m_vertices.count())
    {
        if (m_vertexBuffer)
            delete[] m_vertexBuffer;

        m_vertexBuffer = new float[m_vertices.count() * 8];
        m_vertexCount = m_vertices.count();
        int _offset = 0;
        for (auto &vertex : m_vertices)
        {
            m_vertexBuffer[_offset] = vertex.pos.x(); _offset++;
            m_vertexBuffer[_offset] = vertex.pos.y(); _offset++;
            m_vertexBuffer[_offset] = vertex.pos.z(); _offset++;

            m_vertexBuffer[_offset] = vertex.texture.x(); _offset++;
            m_vertexBuffer[_offset] = vertex.texture.y(); _offset++;

            m_vertexBuffer[_offset] = vertex.normal.x(); _offset++;
            m_vertexBuffer[_offset] = vertex.normal.y(); _offset++;
            m_vertexBuffer[_offset] = vertex.normal.z(); _offset++;
        }
    }

    m_vao.bind();
    m_vbo.bind();
    // 11-03
    m_vbo.allocate(m_vertexBuffer, sizeof(float) * m_vertices.count() * VertexFloatCount);

    m_program->bind();
    m_program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, VertexFloatCount * sizeof(float));
    m_program->enableAttributeArray("vPos");
    m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, VertexFloatCount * sizeof(float));
    m_program->enableAttributeArray("vTexture");
    m_program->setAttributeBuffer("vNormal", GL_FLOAT, 5 * sizeof(float), 3, VertexFloatCount * sizeof(float));
    m_program->enableAttributeArray("vNormal");


    m_program->release();

    m_vbo.release();
    m_vao.release();
}

void Ball::update()
{
}

void Ball::paint()
{
    /* ��͸���� */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);

    for (auto index : m_textures.keys())
    {
        m_textures[index]->bind(index);
    }
    m_vao.bind();
    m_program->bind();
    // �󶨱任����
    m_program->setUniformValue("projection", m_projection);
    m_program->setUniformValue("view", m_camera->view());
    m_program->setUniformValue("model", model());

    // 11-03 �趨�ƹ�λ������ɫ
    m_program->setUniformValue("light.position", m_light->pos());
    m_program->setUniformValue("light.color", m_light->color().redF(), m_light->color().greenF(), m_light->color().blueF());
    auto &_material = m_materials.value(0);
    // 11-03 �趨����
    m_program->setUniformValue("material.ambient", _material.ambient);
    m_program->setUniformValue("material.diffuse", _material.diffuse);
    m_program->setUniformValue("material.specular", _material.specular);
    //    m_program->setUniformValue("material.shininess", _material.shininess);
    m_program->setUniformValue("material.shininess", 2.0f);

    // ��ͨ�����Ρ������ơ�ע�⣺���Ƶ�ʱ����Ҫ�����������һ��Ҫ���ƶ��ٸ�����
    int _index = 0;
    for (int i = 0; i < m_col * (m_row - 1); ++i)
    {
        glDrawArrays(GL_TRIANGLE_FAN, _index, 4);
        _index += 4;
    }

    // ����
//    glDrawArrays(GL_POINTS, 0, m_vertices.size());


    m_program->release();
    m_vao.release();
    for (auto texture : m_textures)
    {
        texture->release();
    }

    /* �ر�͸���� */
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
