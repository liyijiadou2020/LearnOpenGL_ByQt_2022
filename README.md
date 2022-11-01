# 知识：在Qt中使用OpenGL（一~八）
参考来源：https://blog.csdn.net/github_18974657/article/details/122453424

## 一
### Qt中的OpenGL
要怎么说呢，Qt中对OpenGL的使用是进行了很多层的包装的，我尽可能的挑选了Qt原生的对OpenGL的使用方法，也就是继承`QOpenGLWidget`和`QOpenGLExtraFunctions`的方案
【图片1】
大致上，你的用于显示OpenGL渲染结果的窗口，应该是这么一个类，即拥有以下特性：
- 继承QOpenGLWidget和QOpenGLExtraFunctions
- 重载void initializeGL(),void paintGL()还有void resizeGL(int w, int h)三个函数
- 提供一些函数，用于设置你的摄像机的位置与旋转
- 提供一些函数，用于设置你需要显示的内容具体数据（在本例子中，就是提供点云的具体数据。在其他例子中，可能就是提供模型数据了。比如加载模型，卸载模型等）
- 提供一些函数，用于设置你要显示内容的缩放，旋转与位移（在本例子中，就是点云。在其它例子中，可能就是设置加载的模型的缩放，位置，旋转了）
注意：当你需要显示复杂的内容时，就需要额外的编写逻辑，例如**模型管理**，贴图管理等等。本例子中因为只显示一个点云数据，所以并没有什么复杂的资源管理逻辑，只需要一个顶点缓存用来保存点云数据，然后在有一些矩阵数据保存各种变化矩阵即可。


### initializeGL

在`void initializeGL()`函数中，我们需要对OpenGL进行一定的初始化。
具体来说，就是：

1. 初始化函数指针以便可以使用OpenGL的各种函数。
Qt帮我们提供了这么一个函数：`initializeOpenGLFunctions()`只要调用就可以了。如果不调用这个函数，则我们将无法使用OpenGL提供的函数。
2. 启用一些flag
例如`glEnable(GL_DEPTH_TEST)`启用深度测试等。深度测试可以让我们在绘制3D图形的时候，总是看到“近处的物体可以挡住远处的物体”这个效果。
3. 进行一些缓存的创建。
OpenGL使用的时候需要一些缓存对象，顶点队列对象（VAO），顶点缓存对象（VBO）之类的，都可以在初始化的时候进行创建。
至于什么是VAO，VBO我们之后再说。目前只需要知道OpenGL在3D绘制的时候，其数据的来源，就是来自于这些缓存之中。
4. 初始化shader程序并链接：
Qt提供了一个类`QOpenGLShaderProgram`，我们用它就可以了。
new出来这个类，然后用`addShaderFromSourceCode`或`addShaderFromSourceFile`加载你的shader，最后用link函数连接即可。
至于shader是干什么用的我们之后再说。目前只需要知道shader可以让OpenGL在3D绘制的时候，对数据来源进行一定程度的修改。例如你感受到的**3D空间的位置的变化或旋转实际上都是通过shader对要绘制的模型的顶点进行了额外处理模拟出来的**，并不是真的存在一个摄像机，而是模拟出来了一个摄像机。
5. 根据需要，如果你要显示的内容是静态的（例如一个静态的模型），那么就可以在初始化的时候加载，否则就需要实时的加载了。我们的点云数据是动态变化的，所以不能初始化加载，而是动态加载。

### paintGL

在void paintGL()函数中，我们就可以进行主要的**3D绘制逻辑**了。
我们主要需要进行以下行为：
1. 清除之前绘制的内容，包含颜色与深度信息
```
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

2. 启用shader，调用bind函数
```
m_program->bind();
```

3. 设置shader中的全局数据，例如摄像机矩阵，投影矩阵，每个模型自身的变换矩阵等
```
	// 设置视图矩阵
	m_view.setToIdentity();
	QVector3D _lookDir{ 0, 0, 1 };
	QVector3D _up{ 0, -1, 0 };
	_lookDir = _lookDir * m_cameraRotate;
	_up = _up * m_cameraRotate;
	m_view.lookAt(m_cameraPos, m_cameraPos + _lookDir, _up);
	m_program->setUniformValue("view", m_view);
	// 设置投影矩阵
	m_program->setUniformValue("projection", m_projection);
	// 针对本模型, 设置模型矩阵
	m_model.setToIdentity();
	m_model *= m_scale;
	m_model *= m_rotate;
	m_model *= m_translate;
	m_program->setUniformValue("model", m_model);
```

4. 启用顶点队列对象（VAO）
	`m_vao.bind();`

5. 【动态加载数据】启用并设置顶点缓存（VBO），即告诉OpenGL当前要绘制的顶点信息和其它信息
```
	// 绑定本模型需要的顶点缓冲
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo.allocate(m_vertices, m_vertecesCount * 3 * sizeof(float));
```

6. 【动态加载数据】将shader与顶点缓存进行对应，即告知OpenGL要如何使用将顶点缓存中的数据转化为shader中的顶点数据
```
		// 针对本模型, 设置如何渲染
		m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
		m_program->enableAttributeArray(0);
```

7. 调用OpenGL的绘制函数，绘制顶点缓存中的内容
```
		// 绘制模型
		glDrawArrays(GL_POINTS, 0, m_vertecesCount);
```

8. 释放顶点队列对象（VAO）
	`m_vao.release();`

9. 释放shader
	`m_program->release();`

其中，第5步和第6步就是动态加载数据的步骤。如果你要显示的内容是静态的（例如一个静态模型），你实际上应该在初始化的时候进行这个操作。注意，初始化的时候也要**先启用shader和VAO后再操作**，并**在最后释放VAO和shader**。

注意：可以存在多个缓存。每个缓存可以保存不同的内容（例如不同的模型），根据需要，我们可以在绘制阶段重复4，5，6，7，8步骤以达到绘制不同模型的目的。






















---
# LearnOpenGL_ByQt_2022
This rep is for demo code and test code, which is written for learning OpenGL.

# 任务编号 1

该工作的目的是熟悉矢量图形的基础知识，并获得使用图形 API 和三维图形基元的基本功能的技能。
需要使用库的标准函数（OpenGL 或 Direct3D）来描绘指定的对象并执行必要的转换。

任务30：
绘制一个线框圆环并将其放置在一个线框立方体中。自行设置屏幕上图元的大小和位置。
1. 以 0.3 倍缩放环面
2.画一个圆锥、一个球和一个立方体，其中圆锥的顶点是球的中心，立方体放在球的内部（立方体的中心和球的中心重合）。
3. 移动球体，使其中心与圆锥底的中心重合

# 任务编号 2

这项工作的目的是熟悉描述对象材料属性并允许您设置光源参数的主要 API 函数。

需要开发一个程序来描述一组给定的具有指定材料属性和光源参数的三个对象。同时，在您的第 1 号任务版本中指定的 3D 图元充当一组基本对象。其中一个对象应具有透明度属性（参数值应高于0.5）。另一个选定的对象应模拟抛光表面（光泽度，该值由最大值表示）。作为这样的对象，应该选择具有凸面的图元，例如圆柱体、圆环体、圆锥体、球体、茶壶。第三个对象应该是漫散射的，无光泽的。
场景中必须至少有一个光源，并且能够更改其参数：位置、强度、照明颜色。
最后阶段是对其中一个对象（蒙版）进行纹理处理。也可以使用凹凸映射来使用法线的微失真

作业示例：
例如，它在您的第一个任务版本中：
1. 绘制一个线框立方体和一个线框球体，它们彼此相距一定距离。
2. 将立方体的一个顶点与球心结合起来。
3. 画一个圆环面和一个圆柱体。自己设置图元的大小和位置。
4. 依次先绕 X 轴旋转圆柱体，然后拉伸环面 2 次。
因此，第二个任务的变体可能如下所示：
基本组是 - 一个立方体、一个球体、一个圆柱体、一个圆环。
我们描绘了一个半透明的球体、一个抛光的圆环、一个有纹理的立方体。
打开光源 - 例如，点，白色。我们展示了它的运动，例如，改变灯光的颜色。
