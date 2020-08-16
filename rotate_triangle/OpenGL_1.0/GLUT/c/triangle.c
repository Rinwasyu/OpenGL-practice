#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

GLuint triangle_call_list;
double degree = 0;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(0, 0, -2);
	glRotated(degree*180/3.14, 0, 0, -1);
	glCallList(triangle_call_list);
	glFlush();
}

void timer(int value) {
	degree += 0.001;
	display();
	glutTimerFunc(1, timer, value);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	
	glutCreateWindow("Hello OpenGL 1.0");
	glutReshapeWindow(800, 800);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 0.1, 10);
	glMatrixMode(GL_MODELVIEW);
	
	float vertices[] = {
		0.0, 0.57735, 0.0, 1.0,
		-0.5, -0.28868, 0.0, 1.0,
		0.5, -0.28868, 0.0, 1.0
	};
	
	triangle_call_list = glGenLists(1);
	glNewList(triangle_call_list, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	GLfloat color[] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	for (int i = 0; i < 3; i++) {
		glVertex4fv((vertices+4*i));
	}
	glEnd();
	glEndList();
	
	glutDisplayFunc(display);
	glutTimerFunc(10, timer, 0);
	
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	
	glutMainLoop();
	
	return EXIT_SUCCESS;
}