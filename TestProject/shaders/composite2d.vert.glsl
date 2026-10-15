#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec4 a_secondaryColor;
layout (location = 3) in vec2 a_textureCoord;
layout (location = 4) in vec2 a_localPosition;
layout (location = 5) in float a_textureIndex;
layout (location = 6) in float a_shapeType;
layout (location = 7) in float a_borderThickness;
layout (location = 8) in float a_innerRadius;
layout (location = 9) in float a_radius;

uniform mat4 u_viewProjectionMatrix;

out vec3 v_position;
out vec4 v_color;
out vec4 v_secondaryColor;
out vec2 v_textureCoord;
out vec2 v_localPosition;
out flat float v_textureIndex;
out flat float v_shapeType;
out float v_borderThickness;
out float v_innerRadius;
out float v_radius;

void main()
{
	v_position = a_position;
	v_color = a_color;
	v_secondaryColor = a_secondaryColor;
	v_textureCoord = a_textureCoord;
	v_localPosition = a_localPosition;
	v_textureIndex = a_textureIndex;
	v_shapeType = a_shapeType;
	v_borderThickness = a_borderThickness;
	v_innerRadius = a_innerRadius;
	v_radius = a_radius;

	gl_Position = u_viewProjectionMatrix * vec4(a_position, 1.0);
}
