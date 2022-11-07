//#include <GL/glut.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//#define X .525731112119133606 //使原点到每个顶点的距离均为1.0
//#define Z .850650808352039932
//
//GLfloat LightPosition[] = { 5.0f, 5.0f, 0.0f, 1.0f }; //光源位置
//GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //环境光参数
//GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //漫射光参数
//
//GLfloat xrot; //绕轴旋转
//GLfloat yrot;
//GLfloat zrot;
//
//int width; //窗口大小
//int height;
//
////顶点坐标
//static GLfloat vdata[12][3] = {
//	{ -X, 0.0, Z }, { X, 0.0, Z }, { -X, 0.0, -Z }, { X, 0.0, -Z },
//	{ 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, { 0.0, -Z, -X },
//	{ Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X, 0.0 }
//};
////三角形的面
//static GLuint tindices[20][3] = {
//	{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
//	{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
//	{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
//	{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}
//};
//
//void normalize(float v[3]); //计算两个向量的规范化向量积
//void normcrossprod(float v1[3], float v2[3], float out[3]);
//void drawtriangle(float* v1, float* v2, float* v3); //画三角形
//void subdivide(float* v1, float* v2, float* v3); //单次划分三角形
//void subdivide(float* v1, float* v2, float* v3, long depth); //递归求解，多次划分三角形（通过depth控制）
//
//void normalize(float v[3])
//{
//	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);//三个向量的模
//	if (0 == d)
//	{
//		printf("zero length vector!");
//		return;
//	}
//	//单位向量
//	v[0] /= d;
//	v[1] /= d;
//	v[2] /= d;
//}
//
//void normcrossprod(float v1[3], float v2[3], float out[3])
//{
//	out[0] = v1[1] * v2[2] - v1[2] * v1[1];
//	out[1] = v1[2] * v2[0] - v1[0] * v1[2];
//	out[2] = v1[0] * v2[1] - v1[1] * v1[0];
//	normalize(out);
//}
//
//void drawtriangle(float* v1, float* v2, float* v3)
//{
//	glBegin(GL_TRIANGLES);
//	glNormal3fv(v1);
//	glVertex3fv(v1);
//	glNormal3fv(v2);
//	glVertex3fv(v2);
//	glNormal3fv(v3);
//	glVertex3fv(v3);
//	glEnd();
//}
//
//void subdivide(float* v1, float* v2, float* v3) //非递归的单次细分
//{
//	GLfloat v12[3], v23[3], v31[3];
//	GLint i;
//	for (i = 0; i < 3; i++)
//	{
//		v12[i] = (v1[i] + v2[i]) / 2.0;
//		v23[i] = (v2[i] + v3[i]) / 2.0;
//		v31[i] = (v3[i] + v1[i]) / 2.0;
//	}
//	normalize(v12);
//	normalize(v23);
//	normalize(v31);
//	drawtriangle(v1, v12, v31);
//	drawtriangle(v2, v23, v12);
//	drawtriangle(v3, v31, v23);
//	drawtriangle(v12, v23, v31);
//}
//
//void subdivide(float* v1, float* v2, float* v3, long depth) //递归的depth次细分
//{
//	GLfloat v12[3], v23[3], v31[3];
//	GLint i;
//	if (0 == depth)
//	{
//		drawtriangle(v1, v2, v3);
//		return;
//	}
//	for (i = 0; i < 3; i++)
//	{
//		v12[i] = (v1[i] + v2[i]) / 2.0;
//		v23[i] = (v2[i] + v3[i]) / 2.0;
//		v31[i] = (v3[i] + v1[i]) / 2.0;
//	}
//	normalize(v12);
//	normalize(v23);
//	normalize(v31);
//	subdivide(v1, v12, v31, depth - 1);
//	subdivide(v2, v23, v12, depth - 1);
//	subdivide(v3, v31, v23, depth - 1);
//	subdivide(v12, v23, v31, depth - 1);
//}
//
//void display()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色和深度缓存
//	glLoadIdentity(); //重置当前的模型观察矩阵
//	glTranslatef(0.0f, 0.0f, -5.0f); //移入屏幕5个单位
//	glRotatef(yrot, 0.0f, 1.0f, 0.0f); //饶轴旋转
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //多边形的显示方式
//	glColor3f(0.5f, 0.5f, 0.5f); //灰色
//	glBegin(GL_TRIANGLES);
//	for (int i = 0; i < 20; i++)
//	{
//		glVertex3fv(&vdata[tindices[i][0]][0]);
//		glVertex3fv(&vdata[tindices[i][1]][0]);
//		glVertex3fv(&vdata[tindices[i][2]][0]);
//	}
//	glEnd();
//	for (int i = 0; i < 20; i++)
//	{
//		/*
//		*生成表面的法线向量
//		*/
//		GLfloat d1[3], d2[3], norm[3];
//		for (int j = 0; j < 3; j++)
//		{
//			d1[j] = vdata[tindices[i][0]][j] - vdata[tindices[i][1]][j];
//			d2[j] = vdata[tindices[i][1]][j] - vdata[tindices[i][2]][j];
//		}
//		normcrossprod(d1, d2, norm);
//		glNormal3fv(norm);
//		drawtriangle(&vdata[tindices[i][0]][0],
//						 &vdata[tindices[i][1]][0],
//					 &vdata[tindices[i][2]][0]);
//					 glBegin(GL_TRIANGLES);
//						 glNormal3fv(&vdata[tindices[i][0]][0]);
//						 glVertex3fv(&vdata[tindices[i][0]][0]);
//						 glNormal3fv(&vdata[tindices[i][1]][0]);
//						 glVertex3fv(&vdata[tindices[i][1]][0]);
//						 glNormal3fv(&vdata[tindices[i][2]][0]);
//						 glVertex3fv(&vdata[tindices[i][2]][0]);
//					 glEnd();
//		subdivide(&vdata[tindices[i][0]][0],
//			&vdata[tindices[i][1]][0],
//			&vdata[tindices[i][2]][0],
//			1 //划分次数
//		);
//	}
//	yrot += 0.05f;
//	glutPostRedisplay();
//	glFlush();
//}
//
//void init()
//{
//	glMatrixMode(GL_PROJECTION); //选择投影矩阵
//	glLoadIdentity(); //重置投影矩阵
//	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); //透视效果
//	glMatrixMode(GL_MODELVIEW); //选择模型观察矩阵
//	glLoadIdentity(); //重置模型观察矩阵
//
//	glShadeModel(GL_SMOOTH); //启用阴影平滑
//	glClearColor(0.2f, 0.3f, 0.3f, 0.0f); //设置背景颜色
//	glClearDepth(1.0f); //设置深度缓存
//	glEnable(GL_DEPTH_TEST); //启用深度测试
//	glDepthFunc(GL_LESS); //所作测试的类型
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //透视修正
//
//	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); //设置环境光
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); //设置漫射光
//	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); //设置光源位置
//	glEnable(GL_LIGHT1); //启用1号光源
//	glEnable(GL_LIGHTING); //开启光源
//}
//
//void reshape(int w, int h) //当窗体大小改变时调用
//{
//	width = w;
//	height = h;
//	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //设置窗体的视口大小
//
//	init();
//
//	gluOrtho2D(0.0f, (GLdouble)w, 0.0, (GLdouble)h);  //建立二维可视区域
//}
//
//int main(int argv, char** argc)
//{
//	glutInit(&argv, argc); //glut初始化
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //设置显示模式
//	glutInitWindowSize(300, 300);
//	glutInitWindowPosition(300, 200);
//	glutCreateWindow("二十面体");
//	init();
//	glutDisplayFunc(display); //测试绘图函数
//	glutReshapeFunc(reshape); //注册窗体大小改变函数
//	glutMainLoop(); //GLUT事件处理循环
//	return 0;
//}
