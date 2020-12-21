#include "glSetup.h"

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <glm.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

#include "Postbox.h"
#include "Angles.h"

void Init();
void Quit();
void Render(GLFWwindow* window);
void Keyboard(GLFWwindow* window, int key, int code, int action, int mods);

vec3 eye(3, 3, 3);
vec3 center(0, 0, 0);
vec3 up(0, 0, 1);

float AXIS_LENGTH = 1.75;
float AXIS_LINE_WIDTH = 2;

GLfloat bgColor[4] = { 1,1,1,1 };

float timestep = 1.0f / 60;
float period = 16.0;

bool pause = false;

EulerAngle e_angle;
bool pause1 = false;
bool pause2 = false;
bool pause3 = false;

bool testEulerRotation = false;
bool useEulerRotation = true;
bool useQuaternionRotation = false;


int main(int argc, char* argv[])
{
	vsync = 0;

	GLFWwindow* window = initializeOpenGL(argc, argv, bgColor);
	if (window == NULL) return -1;

	glfwSetKeyCallback(window, Keyboard);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	reshape(window, windowW, windowH);

	Init();

	float previous = (float)glfwGetTime();
	float elapsed = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float now = (float)glfwGetTime();
		float delta = now - previous;
		previous = now;
		elapsed += delta;

		if (elapsed > timestep && !pause)
		{
			if (!pause1) e_angle.yaw = int(e_angle.yaw + 4) % 360;
			else if (!pause2) e_angle.pitch = int(e_angle.pitch + 4) % 360;
			else if (!pause3) e_angle.roll = int(e_angle.roll + 4) % 360;
			elapsed = 0;
		}

		Render(window);
		glfwSwapBuffers(window);
	}
	Quit();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void Init()
{
	//SocketIO::Postbox::instance().CalibrateSensor();
}

void Quit()
{
	// empty
}

void DrawCube()
{
	glPointSize(3 * dpiScaling);

	glBegin(GL_QUADS);
	{
		glColor3f(1, 0, 0);
		glNormal3f(0, 0, 1);	// front
		glVertex3f(0, 0, 1);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(0, 1, 1);

		glNormal3f(0, 0, -1);	// back
		glVertex3f(0, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(0, 0, 0);

		glColor3f(0, 1, 0);
		glNormal3f(1, 0, 0);	// right
		glVertex3f(1, 0, 1);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 1, 1);

		glNormal3f(-1, 0, 0);	// left
		glVertex3f(0, 1, 1);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);

		glColor3f(0, 0, 1);
		glNormal3f(0, 1, 0);	// top
		glVertex3f(0, 1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, 0);
		glVertex3f(0, 1, 0);

		glNormal3f(0, -1, 0);	// bottom
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 0, 1);
		glVertex3f(0, 0, 1);
	}
	glEnd();
}

// https://mathworld.wolfram.com/EulerAngles.html
void EulerRotation(EulerAngle eulerAngles)
{
	cout << '(' << eulerAngles.yaw << ", " << eulerAngles.pitch << ", " << eulerAngles.roll << ')' << endl;
	glRotated(eulerAngles.yaw, 1, 0, 0);	// yaw
	glRotated(eulerAngles.pitch, 0, 1, 0);	// pitch
	glRotated(eulerAngles.roll, 0, 0, 1);	// roll
}

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// http://goldsequence.blogspot.com/2016/04/quaternion-based-rotation-in-opengl.html
void QuaternionRotation(Quaternion q)
{
	cout << '(' << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ')' << endl;
	auto theta = acos(q.w) * 2.0;
	auto norm = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
	norm = norm ? norm : 1;
	glRotated(theta * 180 / M_PI, q.x / norm, q.y * 180 / norm, q.z * 180 / norm);
}

void Render(GLFWwindow* window)
{
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glDisable(GL_LIGHTING);
	drawAxes(AXIS_LENGTH, AXIS_LINE_WIDTH * dpiScaling);

	glPointSize(5 * dpiScaling);
	glLineWidth(5 * dpiScaling);

	glPushMatrix();
	{
		auto sensor = SocketIO::Postbox::instance().GetSensorData();
		auto euler_angle = ComplementaryFilter::Filterate(
			sensor.GetEulerAngleByAccel(),
			sensor.GetEulerAngleByGyro()
		);
		if (!pause)
		{
			if (testEulerRotation)			EulerRotation(e_angle);
			else if (useEulerRotation)		EulerRotation(euler_angle);
			else if (useQuaternionRotation)	QuaternionRotation(euler_angle);
		}
		DrawCube();
	}
	glPopMatrix();
}

void Keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;


		case GLFW_KEY_T:
			testEulerRotation = !testEulerRotation;
			break;

		case GLFW_KEY_E:
			useEulerRotation = !useEulerRotation;
			break;

		case GLFW_KEY_Q:
			useQuaternionRotation = !useQuaternionRotation;
			break;


		case GLFW_KEY_SPACE:
			pause = !pause;
			break;

		case GLFW_KEY_1:
			pause1 = !pause1;
			break;
		case GLFW_KEY_2:
			pause2 = !pause2;
			break;
		case GLFW_KEY_3:
			pause3 = !pause3;
			break;
		}
	}
}