#include "TracingBall.h"
#include <limits>
#include <iostream>
using namespace std;

TracingBall::TracingBall(void)
{
}


TracingBall::~TracingBall(void)
{
}


TracingBall* TracingBall::create(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
	TracingBall* pTracingBall = new TracingBall();
	pTracingBall->m_center[0] = x;
	pTracingBall->m_center[1] = y;
	pTracingBall->m_center[2] = z;
	pTracingBall->m_radius = radius;
	pTracingBall->isRotating = false;
	pTracingBall->isTranslating = false;
	return pTracingBall;
}

bool TracingBall::getIntersectionAtOrtho(GLfloat* result, GLfloat x, GLfloat y)
{
	if( (m_center[0] - x)*(m_center[0] - x) + (m_center[1] - y)*(m_center[1] - y) > m_radius*m_radius  )
	{
		return false;
	}
	else
	{
		result[0] = x;
		result[1] = y;
		result[2] = sqrt(m_radius * m_radius - (m_center[0] - x)*(m_center[0] - x) - (m_center[1] - y)*(m_center[1] - y) );
		return true;
	}
}

GLfloat getModule(GLfloat* vector)
{
	GLfloat sum = 0;
	for(int i=0; i<3; i++)
	{
		sum += vector[i]*vector[i];
	}
	return sqrt(sum);
}

GLfloat getCita(GLfloat* vector1, GLfloat* vector2)
{
	GLfloat sum = 0;
	for(int i=0; i<3; i++)
	{
		sum += vector1[i] * vector2[i];
	}
	
	GLfloat cosCita = sum/(getModule(vector1)*getModule(vector2));

	if(abs(cosCita > 1))
	{
		return 0;
	}
	else
	{
		return acos(cosCita)*180/3.1416;
	}
}

bool isEqual(GLfloat* Point1, GLfloat* Point2)
{
	for(int i=0; i<3; i++)
	{
		if(Point1[i] != Point2[i])
		{
			return false;
		}
	}
	return true;
}

void TracingBall::getRotateMatrix(GLfloat* resultMatrix, GLfloat* srcPoint, GLfloat* desPoint)
{
	GLfloat CitaX, CitaY, CitaZ;

	if(isEqual(srcPoint, desPoint))
	{
		CitaX = 0;
		CitaY = 0;
		CitaZ = 0;
	}
	else
	{
		GLfloat srcVector[3], desVector[3], rotateAxis[3];
		for(int i=0; i<3; i++)
		{
			srcVector[i] = srcPoint[i] - m_center[i];
			desVector[i] = desPoint[i] - m_center[i];
		}

		//²æ³ËËã³öÐý×ªÖá
		rotateAxis[0] = srcVector[1]*desVector[2]-srcVector[2]*desVector[1];
		rotateAxis[1] = srcVector[2]*desVector[0]-srcVector[0]*desVector[2];
		rotateAxis[2] = srcVector[0]*desVector[1]-srcVector[1]*desVector[0];
		GLfloat module = getModule(rotateAxis);
		for(int i =0; i<3; i++)
		{
			rotateAxis[i] = rotateAxis[i]/module;
		}

		//¼ÆËãÈÆZÖáÐý×ª½Ç¶È
		GLfloat vectorProj[3] = {rotateAxis[0], rotateAxis[1], 0};
		GLfloat vectorX[3] = {1,0,0};
		CitaZ = getCita(vectorProj, vectorX);
		if(rotateAxis[1] < 0)
		{
			CitaZ = 360 - CitaZ;
		}

		//¼ÆËãÈÆYÖáÐý×ª½Ç¶È
		CitaY = getCita(rotateAxis, vectorProj);
		if(rotateAxis[2]>0)
		{
			CitaY = -CitaY;
		}

		//¼ÆËãÈÆXÖáÐý×ª½Ç¶È
		CitaX = getCita(srcVector, desVector);
	}

	//¼ÆËãÐý×ª¾ØÕó
	glPushMatrix();
	glTranslated(m_center[0], m_center[1], m_center[2]);
	glRotatef(CitaZ, 0, 0, 1 );
	glRotatef(CitaY, 0, 1, 0 );
	glRotatef(CitaX, 1, 0, 0 );
	glRotatef(CitaY, 0, -1, 0 ); 
	glRotatef(CitaZ, 0, 0, -1 );
	glTranslated(-m_center[0], -m_center[1], m_center[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, resultMatrix);
	glPopMatrix();
}

GLfloat* TracingBall::getCenter()
{
	return m_center;
}

void TracingBall::setCenter(GLfloat *center)
{
	memcpy(m_center, center, sizeof(float)*3);
}
