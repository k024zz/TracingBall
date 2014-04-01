//A.11  用虚拟跟踪球实现立方体旋转程序
/* Rotating Dodecahedron demo with trackball */
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include "TracingBall.h"

#define M_PI 3.14
#define ORTHOUNIT 3.5f
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

using namespace std;

float  angle = 0.0, axis[3], trans[3];
bool  trackingMouse = false;
bool redrawContinue = false;
bool   trackballMove = false;
/* Draw the Dodecahedron */

// Global
int  g_winWidth, g_winHeight;
TracingBall* g_pTracingBall;
GLfloat g_lastPoint[3];

GLfloat g_vertices[][3] = {
	{ 1.214124,	 0.000000, 1.589309},
	{ 0.375185,	 1.154701, 1.589309},
	{-0.982247,	 0.713644, 1.589309},
	{-0.982247,	-0.713644, 1.589309},
	{ 0.375185,	-1.154701, 1.589309},
	{ 1.964494,	 0.000000, 0.375185},
	{ 0.607062,	 1.868345, 0.375185},
	{-1.589309,	 1.154701, 0.375185},
	{-1.589309,	-1.154701, 0.375185},
	{ 0.607062,	-1.868345, 0.375185},
	{ 1.589309,	 1.154701, -0.375185},
	{-0.607062,	 1.868345, -0.375185},
	{-1.964494,	 0.000000, -0.375185},
	{-0.607062,	-1.868345, -0.375185},
	{ 1.589309,	-1.154701, -0.375185},
	{ 0.982247,	 0.713644, -1.589309},
	{-0.375185,	 1.154701, -1.589309},
	{-1.214124,    0.000000, -1.589309},
	{-0.375185,	-1.154701, -1.589309},
	{ 0.982247,	-0.713644, -1.589309}
};

GLfloat g_colors[][3] = {
	{1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
	{0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}
};

// Functions
void appInit();
void myReshape(int w, int h);
void polygon(int a, int b, int c , int d, int e, int face);
void colorDodecahedron(void);
void startRotation(int x, int y);
void stopRotation(int x, int y);
void startTranslation(int x, int y);
void stopTranslation(int x, int y);
void mouseMotion(int x, int y);
void mouseButton(int button, int state, int x, int y);
void matrixMultiply(float* point, float* matrix);
void keyBoard(unsigned char key, int x, int y);
void renderBitmapString(float x, float y, float z, void *font, char *string);
void display(void);


int main(int argc, char **argv)
{
	appInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow("TracingBall");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyBoard);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ORTHOUNIT, ORTHOUNIT, -ORTHOUNIT, ORTHOUNIT, -ORTHOUNIT, ORTHOUNIT);
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	glutMainLoop();
}


void appInit()
{
	g_pTracingBall = TracingBall::create(0, 0, 0, 2);
	g_winHeight = SCREEN_HEIGHT;
	g_winWidth = SCREEN_WIDTH;
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	g_winWidth = w;
	g_winHeight = h;
}


void polygon(int a, int b, int c , int d, int e, int face)
{
	/* draw a polygon via list of vertices */
	glBegin(GL_POLYGON);
	if(face > 6)
	{
		face -= 7;
	}
	glColor3fv(g_colors[face]);
	glVertex3fv(g_vertices[a]);
	glVertex3fv(g_vertices[b]);
	glVertex3fv(g_vertices[c]);
	glVertex3fv(g_vertices[d]);
	glVertex3fv(g_vertices[e]);
	glEnd();
}


void colorDodecahedron(void)
{
	/* map vertices to faces */
	polygon(0,1,2,3,4,0);
	polygon(0,5,10,6,1,1);
	polygon(1,6,11,7,2,2);
	polygon(2,7,12,8,3,3);
	polygon(3,8,13,9,4,4);
	polygon(4,9,14,5,0,5);
	polygon(15,10,5,14,19,6);
	polygon(16,11,6,10,15,7);
	polygon(17,12,7,11,16,8);
	polygon(18,13,8,12,17,9);
	polygon(19,14,9,13,18,10);
	polygon(19,18,17,16,15,11);
}

void mouseButton(int button, int state, int x, int y)
{
	if(button==GLUT_RIGHT_BUTTON) switch(state)
	{
	case GLUT_DOWN:
		y=g_winHeight-y;
		startTranslation(x,y);
		break;
	case GLUT_UP:
		stopTranslation(x,y);
		break;
	}
	else if(button==GLUT_LEFT_BUTTON) switch(state)
	{
	case GLUT_DOWN:
		y=g_winHeight-y;
		startRotation(x,y);
		break;
	case GLUT_UP:
		stopRotation(x,y);
		break;
	}
}

void startRotation(int x, int y)
{
	GLfloat startPoint[3];
	if(g_pTracingBall->getIntersectionAtOrtho(startPoint, 
		((GLfloat)x/g_winWidth*2-1)*ORTHOUNIT,
		((GLfloat)y/g_winHeight*2-1)*ORTHOUNIT))
	{
		if(g_pTracingBall->isTranslating == false)
		{
			g_pTracingBall->isRotating = true;
			memcpy(g_lastPoint, startPoint, sizeof(GLfloat)*3);
		}
	}
}


void stopRotation(int x, int y)
{
	g_pTracingBall->isRotating = false;
	g_pTracingBall->isTranslating = false;
}


void startTranslation(int x, int y)
{
	GLfloat startPoint[3];
	if(g_pTracingBall->getIntersectionAtOrtho(startPoint, 
		((GLfloat)x/g_winWidth*2-1)*ORTHOUNIT,
		((GLfloat)y/g_winHeight*2-1)*ORTHOUNIT))
	{
		g_pTracingBall->isTranslating = true;
		g_pTracingBall->isRotating = false;
		memcpy(g_lastPoint, startPoint, sizeof(GLfloat)*3);
	}
}

void stopTranslation(int x, int y)
{
	g_pTracingBall->isTranslating = false;
}

void mouseMotion(int x, int y)
{
	y = g_winHeight - y;
	GLfloat curPoint[3];
	float matrix[16];

	if(g_pTracingBall->isTranslating == true)
	{
		g_pTracingBall->getIntersectionAtOrtho(curPoint, 
			((GLfloat)x/g_winWidth*2-1)*ORTHOUNIT,
			((GLfloat)y/g_winHeight*2-1)*ORTHOUNIT);

		GLfloat newCenter[3];
		newCenter[0] = curPoint[0] - g_lastPoint[0] + g_pTracingBall->getCenter()[0];
		newCenter[1] = curPoint[1] - g_lastPoint[1] + g_pTracingBall->getCenter()[1];
		newCenter[2] = 0;

		g_pTracingBall->setCenter(newCenter);

		for(int i=0; i<20; i++)
		{
			g_vertices[i][0] = g_vertices[i][0] + curPoint[0] - g_lastPoint[0];
			g_vertices[i][1] = g_vertices[i][1] + curPoint[1] - g_lastPoint[1];
		}

		memcpy(g_lastPoint, curPoint, sizeof(GLfloat)*3);
	}
	else 
	{
		if(g_pTracingBall->getIntersectionAtOrtho(curPoint, 
			((GLfloat)x/g_winWidth*2-1)*ORTHOUNIT,
			((GLfloat)y/g_winHeight*2-1)*ORTHOUNIT))
		{
			if(g_pTracingBall->isRotating == false)
			{
				startRotation(x, y);
			}
			else
			{
				g_pTracingBall->getRotateMatrix(matrix, g_lastPoint, curPoint);
				for(int i=0; i<20; i++)
				{
					matrixMultiply(g_vertices[i], matrix);
				}
				memcpy(g_lastPoint, curPoint, sizeof(GLfloat)*3);
			}
		}
		else
		{
			g_pTracingBall->isRotating = false;
		}
	}
}


void matrixMultiply(float* point, float* matrix)
{
	float x, y, z;
	x = point[0]*matrix[0] + point[1]*matrix[4] + point[2] * matrix[8] + matrix[12];
	y = point[0]*matrix[1] + point[1]*matrix[5] + point[2] * matrix[9]+ matrix[13];
	z = point[0]*matrix[2] + point[1]*matrix[6] + point[2] * matrix[10]+ matrix[14];
	point[0] = x;
	point[1] = y;
	point[2] = z;
}

void keyBoard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '1':
		glPolygonMode(GL_FRONT_AND_BACK   ,GL_FILL);
		break;
	case '2':
		glPolygonMode(GL_FRONT_AND_BACK   ,GL_LINE);
		break;
	}
}

void display(void)
{
	static float time = -30;
	if( glutGet(GLUT_ELAPSED_TIME) - time > 20 ) //FPS为50
	{
		time = glutGet(GLUT_ELAPSED_TIME);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//绘制字体
		glColor3fv(g_colors[5]);
		renderBitmapString(-ORTHOUNIT+0.05, ORTHOUNIT-0.3, 2, GLUT_BITMAP_TIMES_ROMAN_24, "Press '1' to render with surfaces.");
		renderBitmapString(-ORTHOUNIT+0.05, ORTHOUNIT-0.6, 2, GLUT_BITMAP_TIMES_ROMAN_24, "Press '2' to render with frames.");

		if (trackballMove)
		{
			glRotatef(angle, axis[0], axis[1], axis[2]);
		}
		colorDodecahedron();
		glutSwapBuffers();
	}
	glutPostRedisplay();
}

void renderBitmapString(float x, float y, float z, void *font, char *string) 
{
		char *c;
		glRasterPos3f(x, y, z);
		for (c=string; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}
}