import glfw
from OpenGL.GL import *
import numpy as np
import math

def loadFile(file_name):
	with open(file_name, "r") as f:
		return f.read()

def printCompilerInfoLog(shader):
	print("log:", glGetShaderInfoLog(shader))

def createProgram():
	program = glCreateProgram()
	
	vertex_shader_src = loadFile("shader.vert")
	vertex_shader = glCreateShader(GL_VERTEX_SHADER)
	glShaderSource(vertex_shader, vertex_shader_src)
	glCompileShader(vertex_shader)
	printCompilerInfoLog(vertex_shader)
	glAttachShader(program, vertex_shader)
	glDeleteShader(vertex_shader)	
	
	fragment_shader_src = loadFile("shader.frag")
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER)
	glShaderSource(fragment_shader, fragment_shader_src)
	glCompileShader(fragment_shader)
	printCompilerInfoLog(fragment_shader)
	glAttachShader(program, fragment_shader)
	glDeleteShader(fragment_shader)
	
	glLinkProgram(program)
	
	return program

def main():
	if not glfw.init():
		return
	
	glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 4)
	glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 6)
	glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
	
	window = glfw.create_window(800, 800, "Hello OpenGL 4.6", None, None)
	if not window:
		glfw.terminate()
		return
	
	glfw.make_context_current(window)
	
	vertices = np.array([
			0.0, 0.57735, 0.0, 1.0,
			-0.5, -0.28868, 0.0, 1.0,
			0.5, -0.28868, 0.0, 1.0
		], dtype=np.float32)
	
	vao = glGenVertexArrays(1)
	glBindVertexArray(vao)
	glEnableVertexAttribArray(0)
	
	vbo = glGenBuffers(1)
	glBindBuffer(GL_ARRAY_BUFFER, vbo)
	glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_STATIC_DRAW)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, None)
	glBindBuffer(GL_ARRAY_BUFFER, 0)
	
	glBindVertexArray(0)
	
	glClearColor(0, 0, 0, 1)
	glEnable(GL_DEPTH_TEST)
	
	program = createProgram()
	glUseProgram(program)
	glBindVertexArray(vao)
	rotation_location = glGetUniformLocation(program, "rotation")
	
	while not glfw.window_should_close(window):
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
		rotation = [
			math.cos(glfw.get_time()), -math.sin(glfw.get_time()), 0, 0,
			math.sin(glfw.get_time()), math.cos(glfw.get_time()), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		]
		glUniformMatrix4fv(rotation_location, 1, GL_FALSE, rotation)
		glDrawArrays(GL_TRIANGLES, 0, 3)
		glfw.swap_buffers(window)
		glfw.poll_events()
	
	glfw.terminate()
	
	return

if __name__ == "__main__":
	main()