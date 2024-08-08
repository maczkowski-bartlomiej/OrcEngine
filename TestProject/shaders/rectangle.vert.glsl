#version 460 core

layout (location = 0) in vec4 a_fillColor;
layout (location = 1) in vec4 a_borderColor;
layout (location = 2) in vec3 a_position;
layout (location = 3) in vec2 a_textureCoords;
layout (location = 4) in float a_textureIndex;
layout (location = 5) in float a_borderThickness;

uniform mat4 u_viewProjectionMatrix;

out vec4 v_fillColor;
out vec4 v_borderColor;
out vec2 v_textureCoords;
out float v_textureIndex;
out float v_borderThickness;

void main()
{
	v_fillColor = a_fillColor;
	v_borderColor = a_borderColor;
	v_textureIndex = a_textureIndex;
	v_textureCoords = a_textureCoords;
	v_borderThickness = a_borderThickness;

	gl_Position = u_viewProjectionMatrix * vec4(a_position, 1.0);
}
