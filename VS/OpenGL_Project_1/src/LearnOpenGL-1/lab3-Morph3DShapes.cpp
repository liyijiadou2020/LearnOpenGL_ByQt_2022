//#include <windows.h>       
//#include <math.h>          
//#include <stdio.h>         
//#include <stdlib.h>        
//#include <gl/GL.h>      
//#include <gl/GLU.h>        
//#include <gl/glut.h>
//
//static GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//static GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.9f };
//static GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//static GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
//
//static GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//static GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//static GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//static GLfloat high_shininess[] = { 100.0f };
//
//typedef struct point_3d {  // ������ܧ���� �էݧ� 3D ����ܧ�
//	double x, y, z;
//} POINT_3D;
//
//typedef struct bpatch {    // ������ܧ���� �էݧ� ���ݧڧߧ�ާ� ���ѧԧާ֧ߧ�� ���֧٧�� 3 ���֧�֧ߧ� 
//	POINT_3D  anchors[4][4]; // ���֧�ܧ� 4x4 �ѧߧܧ֧�ߧ�� (anchor) ����֧�
//	GLuint    dlBPatch;      // ����ڧ��� �էݧ� ���ѧԧާ֧ߧ�� ���֧٧��
//	GLuint    texture;       // ���֧ܧ����� �էݧ� ���ѧԧާ֧ߧ��
//	POINT_3D  center;
//} BEZIER_PATCH;
//
//HDC       hDC = NULL;        // ����ߧ�֧ܧ�� ������ۧ��ӧ�
//HGLRC     hRC = NULL;        // ����ߧ�֧ܧ�� �ӧڧ٧�ѧݧڧ٧ѧ�ڧ�
//HWND      hWnd = NULL;       // ���֧�ܧ�ڧ���� ��ܧߧ�
//HINSTANCE hInstance;       // ���ܧ٧֧ާ�ݧ�� ���ڧݧ�ا֧ߧڧ�
//
//bool      keys[256];       // ���ѧ��ڧ� �էݧ� ��ѧҧ��� �� �ܧݧѧӧڧѧ�����
//bool      active = TRUE;     // ���ݧѧ� �ѧܧ�ڧӧߧ���� ���ڧݧ�ا֧ߧڧ�
//bool      fullscreen = TRUE; // ���ݧѧ� ���ݧߧ��ܧ�ѧߧߧ�ԧ� ��֧اڧާ�
//bool      showCPoints = TRUE;
//
//DEVMODE      DMsaved;      // ������ѧߧڧ�� �ߧѧ����ۧܧ� ���֧է�է��֧ԧ� ��֧اڧާ� 
//
//GLfloat      rotz = 0.0f;  // ����ѧ�֧ߧڧ� ��� ���� Z
//BEZIER_PATCH    sphere[8];
//BEZIER_PATCH    flat[8];
//BEZIER_PATCH    process[8][8];
//BEZIER_PATCH    current[8];
//int current_num = 0;
//
//int      divs = 7;         // ���ڧ�ݧ� �ڧߧ�֧���ݧ��ڧ� (����ߧ���ݧ� ��ѧ٧�֧�֧ߧڧ� ���ݧڧԧ�ߧ�) ( ���������� )
//
//LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);      // ���֧ܧݧѧ�ѧ�ڧ� �էݧ� WndProc
//// ���ݧ�اڧ�� 2 ����ܧ�.
//POINT_3D pointAdd(POINT_3D p, POINT_3D q) {
//	p.x += q.x;    p.y += q.y;    p.z += q.z;
//	return p;
//}
//
//// ���ާߧ�ا֧ߧڧ� ����ܧ� �ߧ� �ܧ�ߧ��ѧߧ��
//POINT_3D pointTimes(double c, POINT_3D p) {
//	p.x *= c;  p.y *= c;  p.z *= c;
//	return p;
//}
//
//// ����ߧܧ�ڧ� �էݧ� ������֧ߧڧ� ���٧էѧߧڧ� ����ܧ�
//POINT_3D makePoint(double a, double b, double c) {
//	POINT_3D p;
//	p.x = a;  p.y = b;  p.z = c;
//	return p;
//}
//// �����ڧ�ݧ�֧� ���ݧڧߧ�� 3 ���֧�֧ߧ� �ߧ� ���ߧ�ӧѧߧڧ� �ާѧ��ڧӧ� �ڧ� 4 ����֧�
//// �� ��֧�֧ާ֧ߧߧ�� u, �ܧ����ѧ� ��ҧ��ߧ� �ڧ٧ާ֧ߧ�֧��� ��� 0 �է� 1
//POINT_3D Bernstein(float u, POINT_3D* p) {
//	POINT_3D  a, b, c, d, r;
//
//	a = pointTimes(pow(u, 3), p[0]);
//	b = pointTimes(3 * pow(u, 2) * (1 - u), p[1]);
//	c = pointTimes(3 * u * pow((1 - u), 2), p[2]);
//	d = pointTimes(pow((1 - u), 3), p[3]);
//
//	r = pointAdd(pointAdd(a, b), pointAdd(c, d));
//
//	return r;
//}
//// ����٧էѧߧڧ� ���ڧ�ܧ�� ����ҧ�ѧا֧ߧڧ� �ߧ� ���ߧ�ӧ� �էѧߧߧ�� ���ѧԧާ֧ߧ��
//// �� ��ڧ�ݧ� ��ѧ٧ҧڧ֧ߧڧ�
//GLuint genBezier(BEZIER_PATCH patch, int divs) {
//	int    u = 0, v;
//	float    py, px, pyold;
//	GLuint    drawlist = glGenLists(1);  // ����٧էѧ�� ���ڧ��� ����ҧ�ѧا֧ߧڧ�
//	POINT_3D  temp[4];
//	POINT_3D* last = (POINT_3D*)malloc(sizeof(POINT_3D) * (divs + 1));
//	// ���ѧ��ڧ� ����֧� �էݧ� ���ާ֧�ܧ� ��֧�ӧ�� �ݧڧߧڧ� ���ݧڧԧ�ߧ��
//
//	if (patch.dlBPatch != NULL)   // ���էѧݧڧ�� ���ѧ��� ���ڧ�ܧ� ����ҧ�ѧا֧ߧڧ�
//		glDeleteLists(patch.dlBPatch, 1);
//
//	temp[0] = patch.anchors[0][3];// ���֧�ӧѧ� ����ڧ٧ӧ�էߧѧ� �ܧ�ڧӧѧ� (���է�ݧ� ���� X)
//	temp[1] = patch.anchors[1][3];
//	temp[2] = patch.anchors[2][3];
//	temp[3] = patch.anchors[3][3];
//
//	for (v = 0; v <= divs; v++) {   // ����٧էѧߧڧ� ��֧�ӧ�� �ݧڧߧڧ� ����֧�
//		px = ((float)v) / ((float)divs);   // ������֧ߧ� �ӧէ�ݧ� ���� Y
//		// ������ݧ�٧�֧� 4 ����ܧ� �ڧ� ����ڧ٧ӧ�էߧ�� �ܧ�ڧӧ�� �էݧ� �ӧ��ڧ�ݧ֧ߧڧ� ����֧� �ӧէ�ݧ� �ܧ�ڧӧ��
//		last[v] = Bernstein(px, temp);
//	}
//
//	glNewList(drawlist, GL_COMPILE);     // ���ѧ�ߧ֧� �ߧ�ӧ�� ���ڧ��� ����ҧ�ѧا֧ߧڧ�
//	glBindTexture(GL_TEXTURE_2D, patch.texture); // ����ڧ��֧էڧߧڧ� �� ��֧ܧ�����
//
//	for (u = 1; u <= divs; u++) {
//		py = ((float)u) / ((float)divs);  // ������֧ߧ� �ӧէ�ݧ� ���� Y1
//		pyold = ((float)u - 1.0f) / ((float)divs); // ������֧ߧ� �ӧէ�ݧ� ���ѧ��� ���� Y
//
//		temp[0] = Bernstein(py, patch.anchors[0]); // �����ڧ�ݧڧ� �ߧ�ӧ�� ����ܧ� ���֧٧��
//		temp[1] = Bernstein(py, patch.anchors[1]);
//		temp[2] = Bernstein(py, patch.anchors[2]);
//		temp[3] = Bernstein(py, patch.anchors[3]);
//
//		glBegin(GL_TRIANGLE_STRIP); // ���ѧ�ߧ֧� �ߧ�ӧ�� ���ݧ��ܧ� ���֧�ԧ�ݧ�ߧڧܧ��
//		for (v = 0; v <= divs; v++) {
//			px = ((float)v) / ((float)divs); // ������֧ߧ� �ӧէ�ݧ� ���� X
//
//			glTexCoord2f(pyold, px); // ����ڧާ֧ߧڧ� ���ѧ��� �ܧ���էڧߧѧ�� ��֧ܧ�����
//
//			double length = sqrt(pow(last[v].x, 2) + pow(last[v].y, 2) + pow(last[v].z, 2));
//			glNormal3f(last[v].x / fabs(length), last[v].y / fabs(length), last[v].z / fabs(length));
//
//			glVertex3d(last[v].x, last[v].y, last[v].z);// ����ѧ�ѧ� ����ܧ�
//
//			last[v] = Bernstein(px, temp);   // ���֧ߧ֧�ڧ��֧� �ߧ�ӧ�� ����ܧ�
//			glTexCoord2f(py, px); // ����ڧާ֧ߧڧ� �ߧ�ӧ�� �ܧ���էڧߧѧ�� ��֧ܧ�����
//			glVertex3d(last[v].x, last[v].y, last[v].z); // ����ӧѧ� ����ܧ�
//		}
//
//		glEnd();            // ����ߧ֧� ���ݧ��ܧ� ���֧�ԧ�ݧ�ߧڧܧ��
//	}
//
//	glEndList();          // ����ߧ֧� ���ڧ�ܧ�
//
//	free(last);           // ����ӧ�ҧ�էڧ�� ���ѧ��� �ާѧ��ڧ� �ӧ֧��ڧ�
//	return drawlist;      // ���֧�ߧ��� ���ڧ��� ����ҧ�ѧا֧ߧڧ�
//}
//BEZIER_PATCH initSphereQuarter(point_3d c, point_3d v, double r) {
//
//	BEZIER_PATCH mybezier;
//	double L = 4 / 3 * tan((double)1 / 4 * 90 * 3.14 / 180) * r * 1.2;
//	double l = 4 / 3 * tan((double)1 / 4 * 90 * 3.14 / 180) * L;
//
//	mybezier.anchors[0][3] = makePoint(c.x + v.x * r, c.y + v.y * 0, c.z + v.z * 0);
//	mybezier.anchors[0][2] = makePoint(c.x + v.x * r, c.y + v.y * 0, c.z + v.z * L);
//	mybezier.anchors[0][1] = makePoint(c.x + v.x * L, c.y + v.y * 0, c.z + v.z * r);
//	mybezier.anchors[0][0] = makePoint(c.x + v.x * 0, c.y + v.y * 0, c.z + v.z * r);
//
//	mybezier.anchors[1][3] = makePoint(c.x + v.x * r, c.y + v.y * L, c.z + v.z * 0);
//	mybezier.anchors[1][2] = makePoint(c.x + v.x * r, c.y + v.y * L, c.z + v.z * L);
//	mybezier.anchors[1][1] = makePoint(c.x + v.x * L, c.y + v.y * l, c.z + v.z * r);
//	mybezier.anchors[1][0] = makePoint(c.x + v.x * 0, c.y + v.y * 0, c.z + v.z * r);
//
//	mybezier.anchors[2][3] = makePoint(c.x + v.x * L, c.y + v.y * r, c.z + v.z * 0);
//	mybezier.anchors[2][2] = makePoint(c.x + v.x * L, c.y + v.y * r, c.z + v.z * L);
//	mybezier.anchors[2][1] = makePoint(c.x + v.x * l, c.y + v.y * L, c.z + v.z * r);
//	mybezier.anchors[2][0] = makePoint(c.x + v.x * 0, c.y + v.y * 0, c.z + v.z * r);
//
//	mybezier.anchors[3][3] = makePoint(c.x + v.x * 0, c.y + v.y * r, c.z + v.z * 0);
//	mybezier.anchors[3][2] = makePoint(c.x + v.x * 0, c.y + v.y * r, c.z + v.z * L);
//	mybezier.anchors[3][1] = makePoint(c.x + v.x * 0, c.y + v.y * L, c.z + v.z * r);
//	mybezier.anchors[3][0] = makePoint(c.x + v.x * 0, c.y + v.y * 0, c.z + v.z * r);
//	int i, j;
//	if (v.x * v.y * v.z < 0) {
//		for (i = 0; i < 4; i++) {
//			point_3d a = mybezier.anchors[i][0];
//			point_3d b = mybezier.anchors[i][1];
//
//			mybezier.anchors[i][0] = mybezier.anchors[i][3];
//			mybezier.anchors[i][1] = mybezier.anchors[i][2];
//
//			mybezier.anchors[i][3] = a;
//			mybezier.anchors[i][2] = b;
//		}
//	}
//
//	mybezier.dlBPatch = NULL;
//	return mybezier;
//}
//
//void initSphere(point_3d c, double r) {
//	sphere[0] = initSphereQuarter(c, makePoint(1, 1, 1), r);
//	sphere[1] = initSphereQuarter(c, makePoint(1, 1, -1), r);
//	sphere[2] = initSphereQuarter(c, makePoint(1, -1, 1), r);
//	sphere[3] = initSphereQuarter(c, makePoint(1, -1, -1), r);
//	sphere[4] = initSphereQuarter(c, makePoint(-1, 1, 1), r);
//	sphere[5] = initSphereQuarter(c, makePoint(-1, 1, -1), r);
//	sphere[6] = initSphereQuarter(c, makePoint(-1, -1, 1), r);
//	sphere[7] = initSphereQuarter(c, makePoint(-1, -1, -1), r);
//
//	int i;
//	for (i = 0; i < 8; i++) {
//		sphere[i].dlBPatch = genBezier(sphere[i], divs);
//	}
//}
//
//BEZIER_PATCH initFlatQuarter(point_3d c, point_3d v, double r) {
//	//void initBezier(void) {
//
//	BEZIER_PATCH mybezier;
//	r = r * 3.14 / 2;
//	double alpha = 90 / 3;
//	int i, j;
//
//	double d = (v.z > 0) ? 0.0f : 1.0f;
//
//	for (i = 0; i < 4; i++) {
//		for (j = 0; j < 4; j++) {
//			mybezier.anchors[i][j] = makePoint(
//				c.x + v.x * r * ((double)j / 3 + d) * sin(alpha * i * 3.14 / 180),
//				c.y + v.y * r * ((double)j / 3 + d) * cos(alpha * i * 3.14 / 180),
//				c.z);
//		}
//	}
//
//	if (v.x * v.y > 0) {
//		for (i = 0; i < 4; i++) {
//			point_3d a = mybezier.anchors[i][0];
//			point_3d b = mybezier.anchors[i][1];
//
//			mybezier.anchors[i][0] = mybezier.anchors[i][3];
//			mybezier.anchors[i][1] = mybezier.anchors[i][2];
//
//			mybezier.anchors[i][3] = a;
//			mybezier.anchors[i][2] = b;
//		}
//	}
//
//	mybezier.dlBPatch = NULL;
//	return mybezier;
//}
//
//void initFlat(point_3d c, double r) {
//	flat[0] = initFlatQuarter(c, makePoint(1, 1, 1), r);
//	flat[1] = initFlatQuarter(c, makePoint(1, 1, -1), r);
//	flat[2] = initFlatQuarter(c, makePoint(1, -1, 1), r);
//	flat[3] = initFlatQuarter(c, makePoint(1, -1, -1), r);
//	flat[4] = initFlatQuarter(c, makePoint(-1, 1, 1), r);
//	flat[5] = initFlatQuarter(c, makePoint(-1, 1, -1), r);
//	flat[6] = initFlatQuarter(c, makePoint(-1, -1, 1), r);
//	flat[7] = initFlatQuarter(c, makePoint(-1, -1, -1), r);
//
//	int i;
//	for (i = 0; i < 8; i++) {
//		flat[i].dlBPatch = genBezier(flat[i], divs);
//	}
//}
//
//void initProcess(void)
//{
//	int i, j, k, m;
//	for (i = 0; i < 8; i++) {
//		process[0][i] = flat[i];
//		process[7][i] = sphere[i];
//	}
//
//	for (i = 1; i < 7; i++) {
//		for (j = 0; j < 8; j++) {
//			for (k = 0; k < 4; k++) {
//				int kr = 3 - k;
//				for (m = 0; m < 4; m++) {
//					int mr = 3 - m;
//					double hx = sphere[j].anchors[kr][mr].x - flat[j].anchors[k][m].x;
//					double hy = sphere[j].anchors[kr][mr].y - flat[j].anchors[k][m].y;
//					double hz = sphere[j].anchors[kr][mr].z - flat[j].anchors[k][m].z;
//					process[i][j].anchors[k][m] = makePoint(
//						flat[j].anchors[k][m].x + hx * i / 8,
//						flat[j].anchors[k][m].y + hy * i / 8,
//						flat[j].anchors[k][m].z + hz * i / 8
//					);
//				}
//			}
//		}
//		for (j = 0; j < 8; j++) {
//			process[i][j].dlBPatch = genBezier(process[i][j], divs);
//		}
//	}
//	for (i = 0; i < 8; i++)
//		current[i] = process[current_num][i];
//
//}
//
//void InitGL(void)     // ���ѧ����ۧܧ� OpenGL
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // ����� ��֧�ߧ��
//	glClearDepth(1.0f);           // ���ѧ����ۧܧ� �ҧ��֧�� �ԧݧ�ҧڧߧ�
//	glDepthFunc(GL_LEQUAL);       // ���ڧ� ��֧��� �ԧݧ�ҧڧߧ�
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ���ݧ���֧ߧߧ�� �ӧ��ڧ�ݧ֧ߧڧ� ��֧���֧ܧ�ڧӧ�
//
//	initSphere(makePoint(0, 0, 0), 0.5);
//	initFlat(makePoint(0, 0, 0.5), 0.5);
//	initProcess();
//
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//	glCullFace(GL_BACK);
//	glDisable(GL_CULL_FACE);
//	glDepthFunc(GL_LESS);
//	glEnable(GL_DEPTH_TEST);
//
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
//	glEnable(GL_COLOR_MATERIAL);
//	glEnable(GL_LIGHTING);
//	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//	glEnable(GL_NORMALIZE);
//
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//}
//void drawQuarter(BEZIER_PATCH mybezier) {
//	glColor3f(1, 1, 1);
//	glCallList(mybezier.dlBPatch);// ����٧�� ���ڧ�ܧ� ���֧٧��
//													// ����� �ߧ֧�ҧ��էڧާ� ���ݧ�ܧ� �� ���� ��ݧ��ѧ�, �ܧ�ԧէ� ���ѧԧާ֧ߧ� �ڧ٧ާ֧ߧڧݧ��
//	glDisable(GL_LIGHTING);
//	if (showCPoints) {    // ����ݧ� ����ڧ��ӧܧ� ��֧�ܧ� �ӧܧݧ��֧ߧ�
//		int i, j;
//		for (i = 0; i < 4; i++) {    // ���ѧ�ڧ��ӧѧ�� �ԧ��ڧ٧�ߧ�ѧݧ�ߧ�� �ݧڧߧڧ�
//			if (i == 0 || i == 3) {
//				glColor3f(0.0f, 1.0f, 0.0f);
//			}
//			else {
//				glColor3f(1.0f, 0.0f, 0.0f);
//			}
//			glBegin(GL_LINE_STRIP);
//			for (j = 0; j < 4; j++)
//				glVertex3d(mybezier.anchors[i][j].x, mybezier.anchors[i][j].y, mybezier.anchors[i][j].z);
//			glEnd();
//		}
//
//		for (i = 0; i < 4; i++) {    // ���ѧ�ڧ��ӧѧ�� �ӧ֧��ڧܧѧݧ�ߧ�� �ݧڧߧڧ�
//			if (i == 0 || i == 3) {
//				glColor3f(0.0f, 1.0f, 0.0f);
//			}
//			else {
//				glColor3f(1.0f, 0.0f, 0.0f);
//			}
//			glBegin(GL_LINE_STRIP);
//			for (j = 0; j < 4; j++)
//				glVertex3d(mybezier.anchors[j][i].x, mybezier.anchors[j][i].y, mybezier.anchors[j][i].z);
//			glEnd();
//		}
//		glColor3f(0.0f, 1.0f, 1.0f);
//	}
//	glEnable(GL_LIGHTING);
//}
//
//void DrawGLScene() {            // ���է֧�� ��ڧ��֧�
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // ����ڧ��ܧ� ��ܧ�ѧߧ� �� �ҧ��֧�� �ԧݧ�ҧڧߧ�
//	glLoadIdentity();            // ���ҧ��� ��֧ܧ��֧� �ާѧ��ڧ�� �ӧڧէ� �ާ�է֧ݧ�
//	glTranslatef(0.0f, 0.0f, -4.0f);          // ���էӧڧ� �ߧѧݧ֧ӧ� �ߧ� 1.5 �֧էڧߧڧ�� �� �ӧԧݧ�ҧ� ��ܧ�ѧߧ� �ߧ� 6.0
//	glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
//	glRotatef(rotz, 0.0f, 0.0f, 1.0f);         // ����ѧ�֧ߧڧ� ��� ���� Z
//	int i;
//	for (i = 0; i < 8; i++) {
//		drawQuarter(current[i]);
//	}
//	glutSwapBuffers();
//
//}
//
//GLint winSize = 800;
//
//void specialKeys(int key, int x, int y)
//{
//	int i, j;
//	switch (key) {
//	case GLUT_KEY_LEFT:
//		rotz -= 0.8f;
//		break;
//	case GLUT_KEY_RIGHT:
//		rotz += 0.8f;
//		break;
//	case GLUT_KEY_DOWN:
//		current_num++;
//		if (current_num > 7) {
//			current_num = 7;
//		}
//		for (i = 0; i < 8; i++)
//			current[i] = process[current_num][i];
//		break;
//	case GLUT_KEY_UP:
//		current_num--;
//		if (current_num < 0) {
//			current_num = 0;
//		}
//		for (i = 0; i < 8; i++)
//			current[i] = process[current_num][i];
//		break;
//	}
//}
//void keyboardKeys(unsigned char key, int x, int y)
//{
//	int i, j;
//	switch (key) {
//	case 's':
//		divs++;
//		for (j = 0; j < 8; j++) {
//			for (i = 0; i < 8; i++) {
//				process[j][i].dlBPatch = genBezier(process[j][i], divs);
//			}
//		}
//		break;
//	case 't':
//		if (divs > 1) {
//			divs--;
//		}
//		for (j = 0; j < 8; j++) {
//			for (i = 0; i < 8; i++) {
//				process[j][i].dlBPatch = genBezier(process[j][i], divs);
//			}
//		}
//		break;
//	case 'g':
//		showCPoints = !showCPoints;
//		break;
//	}
//}
//
//static void idle(void)
//{
//	glutPostRedisplay();
//}
//
//static void resize(int width, int height)
//{
//	const float ar = (float)width / (float)height;
//
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-ar, ar, -1.0, 1.0, 2, 200.0);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitWindowSize(555, 555);
//	glutInitWindowPosition(0, 0);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//	glutCreateWindow("Lab 3");
//	InitGL();
//
//	glutReshapeFunc(resize);
//	glutDisplayFunc(DrawGLScene);
//	glutSpecialFunc(specialKeys);
//	glutKeyboardFunc(keyboardKeys);
//	glutIdleFunc(idle);
//
//	glutMainLoop();
//	return 0;
//}
