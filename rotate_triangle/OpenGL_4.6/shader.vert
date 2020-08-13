#version 460 core

in vec4 position;
uniform mat4 rotation;

void main(void) {
	gl_Position = rotation * position;
}