#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#define MAX_STRLEN 1024

char *loadFile(char *file_name, size_t *size) {
	FILE *fp = fopen(file_name, "rb");
	if (!fp) {
		printf("cannot open file: %s\n", file_name);
		return NULL;
	}
	
	struct stat stat_buf;
	stat(file_name, &stat_buf);
	
	char *text = (char *)malloc(stat_buf.st_size + 1);
	if (!text)
		return NULL;
	memset(text, 0, stat_buf.st_size + 1);
	fread(text, stat_buf.st_size, 1, fp);
	
	fclose(fp);
	
	if (size)
		*size = stat_buf.st_size;
	
	return text;
}

void printCompilerInfoLog(GLuint shader) {
	GLchar *info_log;
	GLsizei buf_size, length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buf_size);
	info_log = (GLchar *)malloc(buf_size+1);
	memset(info_log, 0, buf_size+1);
	glGetShaderInfoLog(shader, buf_size, &length, info_log);
	fprintf(stderr, "log: %s\n", info_log);
}

GLuint createProgram() {
	GLuint program = glCreateProgram();
	
	char *vertex_shader_src = loadFile("shader.vert", NULL);
	if (vertex_shader_src) {
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, (const char **)&vertex_shader_src, NULL);
		glCompileShader(vertex_shader);
		printCompilerInfoLog(vertex_shader);
		glAttachShader(program, vertex_shader);
		glDeleteShader(vertex_shader);
	}
	
	char *fragment_shader_src = loadFile("shader.frag", NULL);
	if (fragment_shader_src) {
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

int str2int(char *str) {
	int n = 0;
	for (size_t i = 0; i < strlen(str); i++) {
		n = n * 10 + str[i] - '0';
	}
	return n;
}

GLboolean createTextureFromPPM(char *ppm, size_t size) {
	char header[4][MAX_STRLEN+1] = {0};
	for (int cnt = 0, comment = 0; cnt < 4 && size > 0; ppm++, size--) {
		if (!comment) {
			if (*ppm == '#') {
				comment = 1;
				continue;
			} else if (*ppm == '\n' || *ppm == ' ') {
				cnt++;
				continue;
			} else if (strlen(header[cnt]) < MAX_STRLEN) {
				header[cnt][strlen(header[cnt])] = *ppm;
			}
		} else if (*ppm == '\n') {
			comment = 0;
		}
	}
	
	if (strcmp(header[0], "P6")) {
		printf("wrong format: %s\n", header[0]);
		return GL_FALSE;
	}
	
	int width = str2int(header[1]);
	int height = str2int(header[2]);
	
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppm);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	return GL_TRUE;
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
	
	GLfloat vertices[] = {
		0.0, 0.57735, 0.0, 1.0,
		-0.5, -0.28868, 0.0, 1.0,
		0.5, -0.28868, 0.0, 1.0
	};
	
	GLfloat texture_coords[] = {
		0.5, 0.134,
		0.0, 1.0,
		1.0, 1.0
	};
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	GLuint vertex_vbo;
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	
	GLuint texture_vbo;
	glGenBuffers(1, &texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	
	size_t ppm_size;
	char *ppm_data = loadFile("texture/gradation_4x4.ppm", &ppm_size);
	createTextureFromPPM(ppm_data, ppm_size);
	
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