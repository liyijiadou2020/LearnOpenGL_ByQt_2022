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
//typedef struct point_3d {  // §³§ä§â§å§Ü§ä§å§â§Ñ §Õ§Ý§ñ 3D §ä§à§é§Ü§Ú
//	double x, y, z;
//} POINT_3D;
//
//typedef struct bpatch {    // §³§ä§â§å§Ü§ä§å§â§Ñ §Õ§Ý§ñ §á§à§Ý§Ú§ß§à§Þ§Ñ §æ§â§Ñ§Ô§Þ§Ö§ß§ä§Ñ §¢§Ö§Ù§î§Ö 3 §ã§ä§Ö§á§Ö§ß§Ú 
//	POINT_3D  anchors[4][4]; // §³§Ö§ä§Ü§Ñ 4x4 §Ñ§ß§Ü§Ö§â§ß§í§ç (anchor) §ä§à§é§Ö§Ü
//	GLuint    dlBPatch;      // §³§á§Ú§ã§à§Ü §Õ§Ý§ñ §æ§â§Ñ§Ô§Þ§Ö§ß§ä§Ñ §¢§Ö§Ù§î§Ö
//	GLuint    texture;       // §´§Ö§Ü§ã§ä§å§â§Ñ §Õ§Ý§ñ §æ§â§Ñ§Ô§Þ§Ö§ß§ä§Ñ
//	POINT_3D  center;
//} BEZIER_PATCH;
//
//HDC       hDC = NULL;        // §¬§à§ß§ä§Ö§Ü§ã§ä §å§ã§ä§â§à§Û§ã§ä§Ó§Ñ
//HGLRC     hRC = NULL;        // §¬§à§ß§ä§Ö§Ü§ã§ä §Ó§Ú§Ù§å§Ñ§Ý§Ú§Ù§Ñ§è§Ú§Ú
//HWND      hWnd = NULL;       // §¥§Ö§ã§Ü§â§Ú§á§ä§à§â §à§Ü§ß§Ñ
//HINSTANCE hInstance;       // §¿§Ü§Ù§Ö§Þ§á§Ý§ñ§â §á§â§Ú§Ý§à§Ø§Ö§ß§Ú§ñ
//
//bool      keys[256];       // §®§Ñ§ã§ã§Ú§Ó §Õ§Ý§ñ §â§Ñ§Ò§à§ä§í §ã §Ü§Ý§Ñ§Ó§Ú§Ñ§ä§å§â§à§Û
//bool      active = TRUE;     // §¶§Ý§Ñ§Ô §Ñ§Ü§ä§Ú§Ó§ß§à§ã§ä§Ú §á§â§Ú§Ý§à§Ø§Ö§ß§Ú§ñ
//bool      fullscreen = TRUE; // §¶§Ý§Ñ§Ô §á§à§Ý§ß§à§ï§Ü§â§Ñ§ß§ß§à§Ô§à §â§Ö§Ø§Ú§Þ§Ñ
//bool      showCPoints = TRUE;
//
//DEVMODE      DMsaved;      // §³§à§ç§â§Ñ§ß§Ú§ä§î §ß§Ñ§ã§ä§â§à§Û§Ü§Ú §á§â§Ö§Õ§í§Õ§å§ë§Ö§Ô§à §â§Ö§Ø§Ú§Þ§Ñ 
//
//GLfloat      rotz = 0.0f;  // §£§â§Ñ§ë§Ö§ß§Ú§Ö §á§à §à§ã§Ú Z
//BEZIER_PATCH    sphere[8];
//BEZIER_PATCH    flat[8];
//BEZIER_PATCH    process[8][8];
//BEZIER_PATCH    current[8];
//int current_num = 0;
//
//int      divs = 7;         // §¹§Ú§ã§Ý§à §Ú§ß§ä§Ö§â§á§à§Ý§ñ§è§Ú§Ú (§¬§à§ß§ä§â§à§Ý§î §â§Ñ§Ù§â§Ö§ê§Ö§ß§Ú§ñ §á§à§Ý§Ú§Ô§à§ß§Ñ) ( §¯§°§£§°§¦ )
//
//LRESULT  CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);      // §¥§Ö§Ü§Ý§Ñ§â§Ñ§è§Ú§ñ §Õ§Ý§ñ WndProc
//// §³§Ý§à§Ø§Ú§ä§î 2 §ä§à§é§Ü§Ú.
//POINT_3D pointAdd(POINT_3D p, POINT_3D q) {
//	p.x += q.x;    p.y += q.y;    p.z += q.z;
//	return p;
//}
//
//// §µ§Þ§ß§à§Ø§Ö§ß§Ú§Ö §ä§à§é§Ü§Ú §ß§Ñ §Ü§à§ß§ã§ä§Ñ§ß§ä§å
//POINT_3D pointTimes(double c, POINT_3D p) {
//	p.x *= c;  p.y *= c;  p.z *= c;
//	return p;
//}
//
//// §¶§å§ß§Ü§è§Ú§ñ §Õ§Ý§ñ §å§á§â§à§ë§Ö§ß§Ú§ñ §ã§à§Ù§Õ§Ñ§ß§Ú§ñ §ä§à§é§Ü§Ú
//POINT_3D makePoint(double a, double b, double c) {
//	POINT_3D p;
//	p.x = a;  p.y = b;  p.z = c;
//	return p;
//}
//// §£§í§é§Ú§ã§Ý§ñ§Ö§Þ §á§à§Ý§Ú§ß§à§Þ 3 §ã§ä§Ö§á§Ö§ß§Ú §ß§Ñ §à§ã§ß§à§Ó§Ñ§ß§Ú§Ú §Þ§Ñ§ã§ã§Ú§Ó§Ñ §Ú§Ù 4 §ä§à§é§Ö§Ü
//// §Ú §á§Ö§â§Ö§Þ§Ö§ß§ß§à§Û u, §Ü§à§ä§à§â§Ñ§ñ §à§Ò§í§é§ß§à §Ú§Ù§Þ§Ö§ß§ñ§Ö§ä§ã§ñ §à§ä 0 §Õ§à 1
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
//// §³§à§Ù§Õ§Ñ§ß§Ú§Ö §ã§á§Ú§ã§Ü§à§Ó §à§ä§à§Ò§â§Ñ§Ø§Ö§ß§Ú§ñ §ß§Ñ §à§ã§ß§à§Ó§Ö §Õ§Ñ§ß§ß§í§ç §æ§â§Ñ§Ô§Þ§Ö§ß§ä§Ñ
//// §Ú §é§Ú§ã§Ý§Ö §â§Ñ§Ù§Ò§Ú§Ö§ß§Ú§Û
//GLuint genBezier(BEZIER_PATCH patch, int divs) {
//	int    u = 0, v;
//	float    py, px, pyold;
//	GLuint    drawlist = glGenLists(1);  // §³§à§Ù§Õ§Ñ§ä§î §ã§á§Ú§ã§à§Ü §à§ä§à§Ò§â§Ñ§Ø§Ö§ß§Ú§ñ
//	POINT_3D  temp[4];
//	POINT_3D* last = (POINT_3D*)malloc(sizeof(POINT_3D) * (divs + 1));
//	// §®§Ñ§ã§ã§Ú§Ó §ä§à§é§Ö§Ü §Õ§Ý§ñ §à§ä§Þ§Ö§ä§Ü§Ú §á§Ö§â§Ó§à§Û §Ý§Ú§ß§Ú§Ú §á§à§Ý§Ú§Ô§à§ß§à§Ó
//
//	if (patch.dlBPatch != NULL)   // §µ§Õ§Ñ§Ý§Ú§ä§î §ã§ä§Ñ§â§í§Ö §ã§á§Ú§ã§Ü§Ú §à§ä§à§Ò§â§Ñ§Ø§Ö§ß§Ú§ñ
//		glDeleteLists(patch.dlBPatch, 1);
//
//	temp[0] = patch.anchors[0][3];// §±§Ö§â§Ó§Ñ§ñ §á§â§à§Ú§Ù§Ó§à§Õ§ß§Ñ§ñ §Ü§â§Ú§Ó§Ñ§ñ (§£§Õ§à§Ý§î §à§ã§Ú X)
//	temp[1] = patch.anchors[1][3];
//	temp[2] = patch.anchors[2][3];
//	temp[3] = patch.anchors[3][3];
//
//	for (v = 0; v <= divs; v++) {   // §³§à§Ù§Õ§Ñ§ß§Ú§Ö §á§Ö§â§Ó§à§Û §Ý§Ú§ß§Ú§Ú §ä§à§é§Ö§Ü
//		px = ((float)v) / ((float)divs);   // §±§â§à§è§Ö§ß§ä §Ó§Õ§à§Ý§î §à§ã§Ú Y
//		// §ª§ã§á§à§Ý§î§Ù§å§Ö§Þ 4 §ä§à§é§Ü§Ú §Ú§Ù §á§â§à§Ú§Ù§Ó§à§Õ§ß§à§Û §Ü§â§Ú§Ó§à§Û §Õ§Ý§ñ §Ó§í§é§Ú§ã§Ý§Ö§ß§Ú§ñ §ä§à§é§Ö§Ü §Ó§Õ§à§Ý§î §Ü§â§Ú§Ó§à§Û
//		last[v] = Bernstein(px, temp);
//	}
//
//	glNewList(drawlist, GL_COMPILE);     // §¯§Ñ§é§ß§Ö§Þ §ß§à§Ó§í§Û §ã§á§Ú§ã§à§Ü §à§ä§à§Ò§â§Ñ§Ø§Ö§ß§Ú§ñ
//	glBindTexture(GL_TEXTURE_2D, patch.texture); // §±§â§Ú§ã§à§Ö§Õ§Ú§ß§Ú§Þ §Ü §ä§Ö§Ü§ã§ä§å§â§Ö
//
//	for (u = 1; u <= divs; u++) {
//		py = ((float)u) / ((float)divs);  // §±§â§à§è§Ö§ß§ä §Ó§Õ§à§Ý§î §à§ã§Ú Y1
//		pyold = ((float)u - 1.0f) / ((float)divs); // §±§â§à§è§Ö§ß§ä §Ó§Õ§à§Ý§î §ã§ä§Ñ§â§à§Û §à§ã§Ú Y
//
//		temp[0] = Bernstein(py, patch.anchors[0]); // §£§í§é§Ú§ã§Ý§Ú§Þ §ß§à§Ó§í§Ö §ä§à§é§Ü§Ú §¢§Ö§Ù§î§Ö
//		temp[1] = Bernstein(py, patch.anchors[1]);
//		temp[2] = Bernstein(py, patch.anchors[2]);
//		temp[3] = Bernstein(py, patch.anchors[3]);
//
//		glBegin(GL_TRIANGLE_STRIP); // §¯§Ñ§é§ß§Ö§Þ §ß§à§Ó§å§ð §á§à§Ý§à§ã§Ü§å §ä§â§Ö§å§Ô§à§Ý§î§ß§Ú§Ü§à§Ó
//		for (v = 0; v <= divs; v++) {
//			px = ((float)v) / ((float)divs); // §±§â§à§è§Ö§ß§ä §Ó§Õ§à§Ý§î §à§ã§Ú X
//
//			glTexCoord2f(pyold, px); // §±§â§Ú§Þ§Ö§ß§Ú§Þ §ã§ä§Ñ§â§í§Ö §Ü§à§à§â§Õ§Ú§ß§Ñ§ä§í §ä§Ö§Ü§ã§ä§å§â§í
//
//			double length = sqrt(pow(last[v].x, 2) + pow(last[v].y, 2) + pow(last[v].z, 2));
//			glNormal3f(last[v].x / fabs(length), last[v].y / fabs(length), last[v].z / fabs(length));
//
//			glVertex3d(last[v].x, last[v].y, last[v].z);// §³§ä§Ñ§â§Ñ§ñ §ä§à§é§Ü§Ñ
//
//			last[v] = Bernstein(px, temp);   // §¤§Ö§ß§Ö§â§Ú§â§å§Ö§Þ §ß§à§Ó§å§ð §ä§à§é§Ü§å
//			glTexCoord2f(py, px); // §±§â§Ú§Þ§Ö§ß§Ú§Þ §ß§à§Ó§í§Ö §Ü§à§à§â§Õ§Ú§ß§Ñ§ä§í §ä§Ö§Ü§ã§ä§å§â§í
//			glVertex3d(last[v].x, last[v].y, last[v].z); // §¯§à§Ó§Ñ§ñ §ä§à§é§Ü§Ñ
//		}
//
//		glEnd();            // §¬§à§ß§Ö§è §á§à§Ý§à§ã§Ü§Ú §ä§â§Ö§å§Ô§à§Ý§î§ß§Ú§Ü§à§Ó
//	}
//
//	glEndList();          // §¬§à§ß§Ö§è §ã§á§Ú§ã§Ü§Ñ
//
//	free(last);           // §°§ã§Ó§à§Ò§à§Õ§Ú§ä§î §ã§ä§Ñ§â§í§Û §Þ§Ñ§ã§ã§Ú§Ó §Ó§Ö§â§ê§Ú§ß
//	return drawlist;      // §£§Ö§â§ß§å§ä§î §ã§á§Ú§ã§à§Ü §à§ä§à§Ò§â§Ñ§Ø§Ö§ß§Ú§ñ
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
//void InitGL(void)     // §¯§Ñ§ã§ä§â§à§Û§Ü§Ú OpenGL
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // §¶§à§ß §é§Ö§â§ß§í§Û
//	glClearDepth(1.0f);           // §¯§Ñ§ã§ä§â§à§Û§Ü§Ú §Ò§å§æ§Ö§â§Ñ §Ô§Ý§å§Ò§Ú§ß§í
//	glDepthFunc(GL_LEQUAL);       // §´§Ú§á §ä§Ö§ã§ä§Ñ §Ô§Ý§å§Ò§Ú§ß§í
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // §µ§Ý§å§é§ê§Ö§ß§ß§í§Ö §Ó§í§é§Ú§ã§Ý§Ö§ß§Ú§ñ §á§Ö§â§ã§á§Ö§Ü§ä§Ú§Ó§í
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
//	glCallList(mybezier.dlBPatch);// §£§í§Ù§à§Ó §ã§á§Ú§ã§Ü§Ñ §¢§Ö§Ù§î§Ö
//													// §¿§ä§à §ß§Ö§à§Ò§ç§à§Õ§Ú§Þ§à §ä§à§Ý§î§Ü§à §Ó §ä§à§Þ §ã§Ý§å§é§Ñ§Ö, §Ü§à§Ô§Õ§Ñ §æ§â§Ñ§Ô§Þ§Ö§ß§ä §Ú§Ù§Þ§Ö§ß§Ú§Ý§ã§ñ
//	glDisable(GL_LIGHTING);
//	if (showCPoints) {    // §¦§ã§Ý§Ú §à§ä§â§Ú§ã§à§Ó§Ü§Ñ §ã§Ö§ä§Ü§Ú §Ó§Ü§Ý§ð§é§Ö§ß§Ñ
//		int i, j;
//		for (i = 0; i < 4; i++) {    // §¯§Ñ§â§Ú§ã§à§Ó§Ñ§ä§î §Ô§à§â§Ú§Ù§à§ß§ä§Ñ§Ý§î§ß§å§ð §Ý§Ú§ß§Ú§ð
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
//		for (i = 0; i < 4; i++) {    // §¯§Ñ§â§Ú§ã§à§Ó§Ñ§ä§î §Ó§Ö§â§ä§Ú§Ü§Ñ§Ý§î§ß§å§ð §Ý§Ú§ß§Ú§ð
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
//void DrawGLScene() {            // §©§Õ§Ö§ã§î §â§Ú§ã§å§Ö§Þ
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // §°§é§Ú§ã§ä§Ü§Ñ §ï§Ü§â§Ñ§ß§Ñ §Ú §Ò§å§æ§Ö§â§Ñ §Ô§Ý§å§Ò§Ú§ß§í
//	glLoadIdentity();            // §³§Ò§â§à§ã §ä§Ö§Ü§å§ë§Ö§Û §Þ§Ñ§ä§â§Ú§è§í §Ó§Ú§Õ§Ñ §Þ§à§Õ§Ö§Ý§Ú
//	glTranslatef(0.0f, 0.0f, -4.0f);          // §³§Õ§Ó§Ú§Ô §ß§Ñ§Ý§Ö§Ó§à §ß§Ñ 1.5 §Ö§Õ§Ú§ß§Ú§è§í §Ú §Ó§Ô§Ý§å§Ò§î §ï§Ü§â§Ñ§ß§Ñ §ß§Ñ 6.0
//	glRotatef(-75.0f, 1.0f, 0.0f, 0.0f);
//	glRotatef(rotz, 0.0f, 0.0f, 1.0f);         // §£§â§Ñ§ë§Ö§ß§Ú§Ö §á§à §à§ã§Ú Z
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
