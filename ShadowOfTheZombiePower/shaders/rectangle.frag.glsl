#version 460

in vec4 v_color;
in vec2 v_textureCoords;
in float v_textureIndex;

uniform sampler2D u_textures[32];
out vec4 color;

void main()
{
	int textureIndex = int(v_textureIndex);
	if (textureIndex == 32)
	{
		color = v_color;
	}
	else
	{	
		color = v_color * texture(u_textures[textureIndex], v_textureCoords);
	}
}
