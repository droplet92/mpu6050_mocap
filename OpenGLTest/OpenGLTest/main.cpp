#include <stdio.h>
#include <gl\glut.h>
#include <cmath>
#include "glDisplay.h"
#include "glCube.h"
#include "Input.h"
#include "Postbox.h"
#include "dof.h"


GLfloat vertices[][3] = 
{ 
	{-1.0,	-1.0,	-1.0},	// vertex 0
	{1.0,	-1.0,	-1.0},	// vertex 1
	{1.0,	1.0,	-1.0},	// vertex 2
	{-1.0,	1.0,	-1.0},	// vertex 3
	{-1.0,	-1.0,	1.0},	// vertex 4
	{1.0,	-1.0,	1.0},	// vertex 5
	{1.0,	1.0,	1.0},	// vertex 6
	{-1.0,	1.0,	1.0},	// vertex 7
};
GLfloat colors[][3] =
{
	{0.0,	0.0,	0.0},	//color 0
	{1.0,	0.0,	0.0},	//color 1
	{1.0,	1.0,	0.0},	//color 2
	{0.0,	1.0,	0.0},	//color 3
	{0.0,	0.0,	1.0},	//color 4
	{1.0,	0.0,	1.0},	//color 5
	{1.0,	1.0,	1.0},	//color 6
	{0.0,	1.0,	1.0}	//color 7	
};
GLfloat matrix[16];

int window_width = 500;
int window_height = 500;

int mouse_prev_x=0, mouse_prev_y=0;
int mouse_dx=0, mouse_dy=0;

bool left_button_pressed=false;


int main(int argc, char **argv)
{
	SocketIO::Postbox::instance().CalibrateSensor();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("3D cube");

	Init();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(IdleFunc);

	glutKeyboardFunc(KeyFunc);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);

	glutMainLoop();

	return 0;
}