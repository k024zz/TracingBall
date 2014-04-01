#pragma once
#include <math.h>
#include <GL/glut.h>

class TracingBall
{
public:
	static TracingBall* create(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);

	bool getIntersectionAtOrtho(GLfloat* result, GLfloat x, GLfloat y);
	bool isRotating;
	bool isTranslating;
	void getRotateMatrix(GLfloat* resultMatrix, GLfloat* srcPoint, GLfloat* desPoint);
	GLfloat* getCenter();
	void setCenter(GLfloat *center);

	TracingBall(void);
	~TracingBall(void);

private:
	GLfloat m_center[3];
	GLfloat m_radius;
	GLfloat m_rotateAxis[3];
};

