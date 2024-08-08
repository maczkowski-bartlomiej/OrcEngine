#version 460

in vec4 v_color;
in vec2 v_textureCoords;
in float v_textureIndex;

uniform sampler2D u_textures[32];
out vec4 color;

void main()
{
    color = v_color * vec4(1.0, 1.0, 1.0, texture(u_textures[int(v_textureIndex)], v_textureCoords).r);
    
	if (color.a == 0.0)
		discard;
}
