//#include <GL/glut.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//#define X .525731112119133606 //ʹԭ�㵽ÿ������ľ����Ϊ1.0
//#define Z .850650808352039932
//
//GLfloat LightPosition[] = { 5.0f, 5.0f, 0.0f, 1.0f }; //��Դλ��
//GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //���������
//GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //��������
//
//GLfloat xrot; //������ת
//GLfloat yrot;
//GLfloat zrot;
//
//int width; //���ڴ�С
//int height;
//
////��������
//static GLfloat vdata[12][3] = {
//	{ -X, 0.0, Z }, { X, 0.0, Z }, { -X, 0.0, -Z }, { X, 0.0, -Z },
//	{ 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, { 0.0, -Z, -X },
//	{ Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X, 0.0 }
//};
////�����ε���
//static GLuint tindices[20][3] = {
//	{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
//	{1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
//	{3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
//	{10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}
//};
//
//void normalize(float v[3]); //�������������Ĺ淶��������
//void normcrossprod(float v1[3], float v2[3], float out[3]);
//void drawtriangle(float* v1, float* v2, float* v3); //��������
//void subdivide(float* v1, float* v2, float* v3); //���λ���������
//void subdivide(float* v1, float* v2, float* v3, long depth); //�ݹ���⣬��λ��������Σ�ͨ��depth���ƣ�
//
//void normalize(float v[3])
//{
//	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);//����������ģ
//	if (0 == d)
//	{
//		printf("zero length vector!");
//		return;
//	}
//	//��λ����
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
//void subdivide(float* v1, float* v2, float* v3) //�ǵݹ�ĵ���ϸ��
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
//void subdivide(float* v1, float* v2, float* v3, long depth) //�ݹ��depth��ϸ��
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
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����ɫ����Ȼ���
//	glLoadIdentity(); //���õ�ǰ��ģ�͹۲����
//	glTranslatef(0.0f, 0.0f, -5.0f); //������Ļ5����λ
//	glRotatef(yrot, 0.0f, 1.0f, 0.0f); //������ת
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //����ε���ʾ��ʽ
//	glColor3f(0.5f, 0.5f, 0.5f); //��ɫ
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
//		*���ɱ���ķ�������
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
//			1 //���ִ���
//		);
//	}
//	yrot += 0.05f;
//	glutPostRedisplay();
//	glFlush();
//}
//
//void init()
//{
//	glMatrixMode(GL_PROJECTION); //ѡ��ͶӰ����
//	glLoadIdentity(); //����ͶӰ����
//	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); //͸��Ч��
//	glMatrixMode(GL_MODELVIEW); //ѡ��ģ�͹۲����
//	glLoadIdentity(); //����ģ�͹۲����
//
//	glShadeModel(GL_SMOOTH); //������Ӱƽ��
//	glClearColor(0.2f, 0.3f, 0.3f, 0.0f); //���ñ�����ɫ
//	glClearDepth(1.0f); //������Ȼ���
//	glEnable(GL_DEPTH_TEST); //������Ȳ���
//	glDepthFunc(GL_LESS); //�������Ե�����
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //͸������
//
//	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); //���û�����
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); //���������
//	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); //���ù�Դλ��
//	glEnable(GL_LIGHT1); //����1�Ź�Դ
//	glEnable(GL_LIGHTING); //������Դ
//}
//
//void reshape(int w, int h) //�������С�ı�ʱ����
//{
//	width = w;
//	height = h;
//	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //���ô�����ӿڴ�С
//
//	init();
//
//	gluOrtho2D(0.0f, (GLdouble)w, 0.0, (GLdouble)h);  //������ά��������
//}
//
//int main(int argv, char** argc)
//{
//	glutInit(&argv, argc); //glut��ʼ��
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //������ʾģʽ
//	glutInitWindowSize(300, 300);
//	glutInitWindowPosition(300, 200);
//	glutCreateWindow("��ʮ����");
//	init();
//	glutDisplayFunc(display); //���Ի�ͼ����
//	glutReshapeFunc(reshape); //ע�ᴰ���С�ı亯��
//	glutMainLoop(); //GLUT�¼�����ѭ��
//	return 0;
//}
