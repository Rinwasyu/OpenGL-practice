#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

char *loadFile(char *file_name) {
	FILE *fp = fopen(file_name, "r");
	if (fp == NULL)
		return NULL;
	
	struct stat stat_buf;
	stat(file_name, &stat_buf);
	
	char *text = (char *)malloc(stat_buf.st_size + 1);
	if (text == NULL)
		return NULL;
	memset(text, 0, stat_buf.st_size + 1);
	for (char ch, *t = text; (ch = fgetc(fp)) != EOF; t++)
		*t = ch;
	
	fclose(fp);
	
	return text;
}

void printCompilerInfoLog(GLuint shader) {
	GLchar *info_log;
	GLsizei buf_size, length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buf_size);
	info_log = (GLchar *)malloc(buf_size);
	memset(info_log, 0, buf_size);
	glGetShaderInfoLog(shader, buf_size, &length, info_log);
	fprintf(stderr, "log: %s\n", info_log);
}

GLuint createProgram() {
	GLuint program = glCreateProgram();
	
	char *vertex_shader_src = loadFile("shader.vert");
	if (vertex_shader_src != NULL) {
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, (const char **)&vertex_shader_src, NULL);
		glCompileShader(vertex_shader);
		printCompilerInfoLog(vertex_shader);
		glAttachShader(program, vertex_shader);
		glDeleteShader(vertex_shader);
	}
	
	char *fragment_shader_src = loadFile("shader.frag");
	if (fragment_shader_src != NULL) {
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, (const char **)&fragment_shader_src, NULL);
		glCompileShader(fragment_shader);
		printCompilerInfoLog(fragment_shader);
		glAttachShader(program, fragment_shader);
		glDeleteShader(fragment_shader);
	}
	
	glLinkProgram(program);
	
	return program;
}

int main(void) {
	if (!glfwInit())
		return EXIT_FAILURE;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(800, 800, "Hello OpenGL 4.6", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	glfwMakeContextCurrent(window);
	
	glewInit();
	
	float vertices[] = {
		0.0, 0.57735, 0.0, 1.0,
		-0.5, -0.28868, 0.0, 1.0,
		0.5, -0.28868, 0.0, 1.0
	};
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	
	GLuint program = createProgram();
	glUseProgram(program);
	glBindVertexArray(vao);
	GLuint rotation_location = glGetUniformLocation(program, "rotation");
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLfloat rotation[] = {
			cos(glfwGetTime()), -sin(glfwGetTime()), 0, 0,
			sin(glfwGetTime()), cos(glfwGetTime()), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, rotation);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	
	return EXIT_SUCCESS;
}