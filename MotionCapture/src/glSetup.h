#ifndef __GL_SETUP_H_
#define __GL_SETUP_H_

#if defined(__APPLE) && defined(__MACH__)	// macOS
#include <OpenGL/glu.h>
#else										// Windows
#include <Windows.h>
#include <gl/GLU.h>
#endif

#include <GLFW/glfw3.h>

extern float	screenScale;		// Portion of the screen when not using full screen
extern int		screenW, screenH;	// screenScale portion of the screen
extern int		windowW, windowH;	// Framebuffer size
extern float	dpiScaling;			// DPI scaling for HIDPI

extern int		vsync;				// Vertical sync on/off

GLFWwindow* initializeOpenGL(int argc, char* argv[], GLfloat bg[4]);
void		reshape(GLFWwindow* window, int w, int h);
void		setupProjectionMatrix();
void		drawAxes(float l, float w);

#endif // __GL_SETUP_H_
