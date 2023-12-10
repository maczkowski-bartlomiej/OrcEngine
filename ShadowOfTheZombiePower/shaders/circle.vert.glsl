#version 460 core

layout (location = 0) in vec4 a_fillColor;
layout (location = 1) in vec4 a_borderColor;
layout (location = 2) in vec2 a_localPosition;
layout (location = 3) in vec2 a_globalPosition;
layout (location = 4) in vec2 a_textureCoords;
layout (location = 5) in float a_textureIndex;
layout (location = 6) in float a_radius;
layout (location = 7) in float a_thickness;
layout (location = 8) in float a_borderThickness;

uniform mat4 u_viewProjectionMatrix;

out vec4 v_fillColor;
out vec4 v_borderColor;
out vec2 v_localPosition;
out vec2 v_textureCoords;
out float v_textureIndex;
out float v_radius;
out float v_thickness;
out float v_borderThickness;

void main()
{
	v_fillColor = a_fillColor;
	v_borderColor = a_borderColor;
	v_localPosition = a_localPosition;
	v_textureIndex = a_textureIndex;
	v_textureCoords = a_textureCoords;
	v_radius = a_radius;
	v_thickness = a_thickness;
	v_borderThickness = a_borderThickness;

	gl_Position = u_viewProjectionMatrix * vec4(a_globalPosition, 0.0, 1.0);
}
