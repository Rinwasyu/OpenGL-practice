#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	if (!glfwInit())
		return EXIT_FAILURE;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(800, 800, "Hello OpenGL 1.0", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	glfwMakeContextCurrent(window);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1, 0.1, 10);
	glMatrixMode(GL_MODELVIEW);
	
	float vertices[] = {
		0.0, 0.57735, 0.0, 1.0,
		-0.5, -0.28868, 0.0, 1.0,
		0.5, -0.28868, 0.0, 1.0
	};
	
	GLuint triangle_call_list = glGenLists(1);
	glNewList(triangle_call_list, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	GLfloat color[] = {1, 1, 1, 1};
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	for (int i = 0; i < 3; i++) {
		glVertex4fv((vertices+4*i));
	}
	glEnd();
	glEndList();
	
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslated(0, 0, -2);
		glRotated(glfwGetTime()*180/3.14, 0, 0, -1);
		glCallList(triangle_call_list);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return EXIT_SUCCESS;
}