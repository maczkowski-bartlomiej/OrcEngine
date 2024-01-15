#version 460 core

layout (location = 0) in vec4 a_color;
layout (location = 1) in vec3 a_position;

uniform mat4 u_viewProjectionMatrix;

out vec4 v_color;

void main()
{
	v_color = a_color;
	gl_Position = u_viewProjectionMatrix * vec4(a_position, 1.0);
}
