#include "glSetup.h"

#include <string.h>
#include <iostream>
using namespace std;

bool	perspectiveView = true;

float	screenScale = 0.5f;			// Portion of the screen when not using full screen
int		screenW = 0, screenH = 0;	// screenScale portion of the screen
int		windowW, windowH;			// Framebuffer size
float	aspect;						// Aspect ratio = windowW/windowH
float	dpiScaling = 0;				// DPI scaling for HIDPI

int		vsync = 1;					// Vertical sync on/off

float	fovy = 46.4f;				// fovy of	28mm lens in degree

float	nearDist = 1.0f;
float	farDist = 20.0f;

void
errorCallback(int error, const char* description)
{
	cerr << "####" << description << endl;
}

void
setupProjectionMatrix()
{
	if (perspectiveView) gluPerspective(fovy, aspect, nearDist, farDist);
	else glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -nearDist, farDist);
}

// w and h are width and height of the framebuffer
void
reshape(GLFWwindow* window, int w, int h)
{
	aspect = (float)w / h;

	// Set the viewport
	windowW = w;
	windowH = h;
	glViewport(0, 0, w, h);

	// Initialize the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a projection matrix
	setupProjectionMatrix();

	// The Screen size is required for mouse interaction.
	glfwGetWindowSize(window, &screenW, &screenH);
	cerr << "reshape(" << w << ", " << h << ")";
	cerr << " with screen " << screenW << " x " << screenH << endl;
}

GLFWwindow*
initializeOpenGL(int argc, char* argv[], GLfloat bgColor[4])
{
	glfwSetErrorCallback(errorCallback);

	// Init GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);

	// Enable OpenGL 2.1 in OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	glfwWindowHint(GLFW_SAMPLES, 4);	// MASS

	// Create the window
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	int monitorW, monitorH;
	glfwGetMonitorPhysicalSize(monitor, &monitorW, &monitorH);
	cerr << "Status: Monitor " << monitorW << "mm x " << monitorH << "mm" << endl;

	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	if (screenW == 0) screenW = int(videoMode->width	* screenScale);
	if (screenH == 0) screenH = int(videoMode->height	* screenScale);

	GLFWwindow* window = glfwCreateWindow(screenW, screenH, argv[0], NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cerr << "Failed in glfwCreateWindow()" << endl;
		return NULL;
	}

	// Context
	glfwMakeContextCurrent(window);

	// Clear the background ASAP
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glfwSwapBuffers(window);

	// Check the size of the window
	glfwGetWindowSize(window, &screenW, &screenH);
	cerr << "Status: Screen " << screenW << " x " << screenH << endl;

	glfwGetFramebufferSize(window, &windowW, &windowH);
	cerr << "Status: Framnebuffer " << windowW << " x " << windowH << endl;
	
	// DPI scaling
	if (dpiScaling == 0) dpiScaling = float(windowW) / screenW;

	// Callbacks
	glfwSetFramebufferSizeCallback(window, reshape);

	// Get the OpenGL version and renderer
	cout << "Status: Renderer " << glGetString(GL_RENDERER) << endl;
	cout << "Status: Ventor " << glGetString(GL_VENDOR) << endl;
	cout << "Status: OpenGL " << glGetString(GL_VERSION) << endl;

	// Vertical sync ...
	glfwSwapInterval(vsync);	// 0 for immediate mode (Tearing possible)

	return window;
}

// Draw the coordinate frame with the xyz axes
void
drawAxes(float l, float w)
{
	glLineWidth(w);

	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0); // x-axis
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0); // y-axis
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1); // z-axis
	glEnd();
}