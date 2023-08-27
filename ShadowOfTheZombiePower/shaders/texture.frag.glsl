#version 460

in vec4 v_color;
in vec2 v_textureCoords;
in float v_textureIndex;

uniform sampler2D u_textures[64];
out vec4 color;

void main()
{
	color = v_color * texture(u_textures[int(v_textureIndex)], v_textureCoords);
}
