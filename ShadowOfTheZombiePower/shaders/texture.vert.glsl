#version 460 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_textureCoords;
layout (location = 3) in float a_textureIndex;

uniform mat4 u_viewProjectionMatrix;

out vec4 v_color;
out vec2 v_textureCoords;
out float v_textureIndex;

void main()
{
	v_color = a_color;
	v_textureIndex = a_textureIndex;
	v_textureCoords = a_textureCoords;
	gl_Position = u_viewProjectionMatrix * vec4(a_position, 0.0, 1.0);
}
