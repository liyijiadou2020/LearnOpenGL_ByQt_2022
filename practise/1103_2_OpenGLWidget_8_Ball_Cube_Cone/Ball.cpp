#include "Ball.h"

//# TODO 没有法线向量！

Ball::Ball()
	:Model()
{
	QVector3D _top{ 0,1,0 };
	float _step = 1;
	QVector<QVector<QVector3D>> _vertexMatrix;

    // 地球仪绘制法：创建球体的顶点
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

    // 根据这个结果创建球体的顶点
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
			m_vertices	<< Vertex{ { _p0.x(), _p0.y(), _p0.z() }, {(float)x / m_col,		(float)y / m_row}		}
						<< Vertex{ { _p1.x(), _p1.y(), _p1.z() }, {(float)x	/ m_col,		(float)(y + 1) / m_row} }
						<< Vertex{ { _p2.x(), _p2.y(), _p2.z() }, {(float)(x + 1) / m_col,	(float)(y + 1) / m_row} }
						<< Vertex{ { _p3.x(), _p3.y(), _p3.z() }, {(float)(x + 1) / m_col,	(float)y / m_row}		};
		}
	}

    // 加载纹理
    auto _texture = new QOpenGLTexture(QImage(":/pic/world-map.jpg"));
	_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	setTexture(_texture);

	auto _program = new QOpenGLShaderProgram();
	_program->addShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
#version 330 core
in vec3 vPos;
in vec2 vTexture;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	TexCoords = vTexture;

    gl_Position = projection * view * model * vec4(vPos, 1.0);
}
)");
	_program->addShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform sampler2D Texture;

void main()
{
    FragColor = vec4(texture(Texture, TexCoords).rgb * lightColor, 1.0);
} 
)");
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
		m_vertexBuffer = new float[m_vertices.count() * 5];
		m_vertexCount = m_vertices.count();
		int _offset = 0;
		for (auto &vertex : m_vertices)
		{
			m_vertexBuffer[_offset] = vertex.pos.x(); _offset++;
			m_vertexBuffer[_offset] = vertex.pos.y(); _offset++;
			m_vertexBuffer[_offset] = vertex.pos.z(); _offset++;
			m_vertexBuffer[_offset] = vertex.texture.x(); _offset++;
			m_vertexBuffer[_offset] = vertex.texture.y(); _offset++;
		}
	}

	m_vao.bind();
	m_vbo.bind();
	m_vbo.allocate(m_vertexBuffer, sizeof(float) * m_vertices.count() * 5);

	m_program->bind();
	m_program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
	m_program->enableAttributeArray("vPos");
	m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
	m_program->enableAttributeArray("vTexture");
	m_program->release();

	m_vbo.release();
	m_vao.release();
}

void Ball::update()
{
}

void Ball::paint()
{
	for (auto index : m_textures.keys())
	{
		m_textures[index]->bind(index);
	}
	m_vao.bind();
	m_program->bind();
	// 绑定变换矩阵
	m_program->setUniformValue("projection", m_projection);
	m_program->setUniformValue("view", m_camera->view());
	m_program->setUniformValue("model", model());
	// 绑定灯光颜色
	m_program->setUniformValue("lightColor", m_light->color().redF(), m_light->color().greenF(), m_light->color().blueF());	
    // 【通过矩形】来绘制。注意：绘制的时候需要计算出来我们一共要绘制多少个矩形
    int _index = 0;
	for (int i = 0; i < m_col * (m_row - 1); ++i)
	{
		glDrawArrays(GL_TRIANGLE_FAN, _index, 4);
		_index += 4;
	}

	m_program->release();
	m_vao.release();
	for (auto texture : m_textures)
	{
		texture->release();
	}
}
