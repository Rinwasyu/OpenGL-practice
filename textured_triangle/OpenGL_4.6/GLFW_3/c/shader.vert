#version 460 core

in vec4 position;
in vec2 texture_uv;

uniform mat4 rotation;

out vec2 uv;

void main(void) {
	gl_Position = rotation * position;
	uv = texture_uv;
}