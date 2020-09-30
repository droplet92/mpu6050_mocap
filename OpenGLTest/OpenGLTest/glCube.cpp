#include "glCube.h"
#include <GL/freeglut.h>

extern GLfloat vertices[][3];
extern GLfloat colors[][3];


void DrawAxis()
{
	constexpr float length = 3.0f;
	constexpr float linewidth = 1.0f;

	glLineWidth(linewidth);

	glBegin(GL_LINES);
	{
		// X-axis (red)
		{
			glColor3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(length, 0, 0);
		}
		// Y-axis (green)
		{
			glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, length, 0);
		}
		// Z-axis (blue)
		{
			glColor3f(0, 0, 1);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, length);
		}
	}
	glEnd();
}

void DrawPolygon(int a, int b, int c, int d)
{
	glBegin(GL_TRIANGLES);
	{
		glColor3fv(colors[a]);
		glVertex3fv(vertices[a]);

		glColor3fv(colors[b]);
		glVertex3fv(vertices[b]);

		glColor3fv(colors[c]);
		glVertex3fv(vertices[c]);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	{
		glColor3fv(colors[a]);
		glVertex3fv(vertices[a]);

		glColor3fv(colors[c]);
		glVertex3fv(vertices[c]);

		glColor3fv(colors[d]);
		glVertex3fv(vertices[d]);
	}
	glEnd();
}

void ColorCube()
{
	DrawPolygon(4, 5, 6, 7);
	DrawPolygon(0, 3, 2, 1);
	DrawPolygon(1, 2, 6, 5);
	DrawPolygon(0, 4, 7, 3);
	DrawPolygon(3, 7, 6, 2);
	DrawPolygon(0, 1, 5, 4);
}