#version 460 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_color;

uniform mat4 u_viewProjectionMatrix;

out vec4 v_color;

void main()
{
	v_color = a_color;
	gl_Position = u_viewProjectionMatrix * vec4(a_position, 0.0, 1.0);
}
