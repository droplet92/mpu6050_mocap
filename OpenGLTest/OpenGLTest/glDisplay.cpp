#include "glDisplay.h"
#include <GL/freeglut.h>
#include "glCube.h"
#include "Input.h"
#include "Postbox.h"
#include "dof.h"
#include <cstdio>

extern GLfloat matrix[16];

extern int window_width;
extern int window_height;

extern int mouse_dx, mouse_dy;


void Init()
{
	glEnable(GL_DEPTH_TEST);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat temp_matrix[16];

	glPushMatrix();
	{
		auto sensor = SocketIO::Postbox::instance().GetSensorData();
		auto euler_angle = ComplementaryFilter::Filterate(
			sensor.GetEulerAngleByAccel(),
			sensor.GetEulerAngleByGyro()
		) * 0.1;
		// OpenGL coordinate system is a little different with MPU6050.
		glRotatef(euler_angle.roll, 1.0, 0.0, 0.0);
		glRotatef(euler_angle.pitch, 0.0, 0.0, 1.0);
		glRotatef(euler_angle.yaw, 0.0, 1.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, temp_matrix);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glLoadMatrixf(temp_matrix);
		glMultMatrixf(matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

		ColorCube();
		DrawAxis();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glViewport(0, 0, w, h);

	window_width = w;
	window_height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(
		-2.0,
		2.0,
		(w <= h) ? (-2.0 * (GLfloat)h / (GLfloat)w) : (-2.0 * (GLfloat)w / (GLfloat)h),
		(w <= h) ? (2.0 * (GLfloat)h / (GLfloat)w) : (2.0 * (GLfloat)w / (GLfloat)h),
		2.0,
		20.0
	);
	glTranslatef(0, 0, -5);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

void IdleFunc()
{
	glutPostRedisplay();
}
