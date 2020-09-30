#include "MPU6050Input.h"
#include <GL/freeglut.h>

extern GLfloat matrix[16];

extern int mouse_prev_x, mouse_prev_y;
extern int mouse_dx, mouse_dy;

extern bool left_button_pressed;


void KeyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		mouse_dx = 0;
		mouse_dy = 0;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		switch (state)
		{
		case GLUT_DOWN:
			if (left_button_pressed == false)
			{
				left_button_pressed = true;
				mouse_prev_x = x;
				mouse_prev_y = y;
			}
			break;
		case GLUT_UP:
			left_button_pressed = false;
			mouse_dx = 0;
			mouse_dy = 0;
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	mouse_dx = x - mouse_prev_x;
	mouse_dy = y - mouse_prev_y;

	mouse_prev_x = x;
	mouse_prev_y = y;

	glutPostRedisplay();
}
