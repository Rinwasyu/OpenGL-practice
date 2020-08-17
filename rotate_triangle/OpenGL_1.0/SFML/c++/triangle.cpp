#include <SFML/System.hpp>
#include <SFML/Window.hpp>
/* SFML/OpenGL.hpp does not support for GLU (OpenGL Utility Library). */
//#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

int main(void) {
	sf::ContextSettings settings;
	settings.majorVersion = 1;
	settings.minorVersion = 0;
	
	sf::Window window(sf::VideoMode(800, 800), "Hello OpenGL 1.0", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	
	window.setActive(true);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.0, 0.1, 10.0);
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
	
	sf::Clock clock;
	
	while(window.isOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslated(0, 0, -2);
		glRotated(clock.getElapsedTime().asSeconds()*180/3.14, 0, 0, -1);
		glCallList(triangle_call_list);
		window.display();
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
	}
	
	return 0;
}