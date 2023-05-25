#version 460

layout(location = 0) out vec4 color;

uniform vec4 u_color;

in vec3 v_position;

void main()
{
	color = u_color;
}