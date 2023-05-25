#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoords;
			
uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transform;
			
out vec2 v_textureCoords;

void main()
{
	v_textureCoords = a_textureCoords;
	gl_Position = u_viewProjectionMatrix * u_transform * vec4(a_position, 1.0);
}
