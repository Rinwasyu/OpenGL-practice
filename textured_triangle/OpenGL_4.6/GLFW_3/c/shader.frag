#version 460 core

in vec2 uv;

uniform sampler2D texture_sampler;

out vec4 frag_color;

void main(void) {
	frag_color = texture2D(texture_sampler, uv);
}