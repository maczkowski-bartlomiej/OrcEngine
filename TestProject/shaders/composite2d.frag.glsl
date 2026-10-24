#version 460 core

in vec3 v_position;
in vec4 v_color;
in vec4 v_secondaryColor;
in vec2 v_textureCoord;
in vec2 v_localPosition;
in flat float v_textureIndex;
in flat float v_shapeType;
in float v_borderThickness;
in float v_innerRadius;
in float v_radius;

uniform sampler2D u_textures[32];

out vec4 o_color;

void main()
{
	int texIndex = int(v_textureIndex);
	int shapeType = int(v_shapeType);

	// Shape Type 0: Sprite / Textured Quad
	if (shapeType == 0)
	{
		vec4 texColor = (texIndex >= 0 && texIndex < 32) ? texture(u_textures[texIndex], v_textureCoord) : vec4(1.0);
		o_color = v_color * texColor;
		if (o_color.a == 0.0)
			discard;
	}
	// Shape Type 1: Rectangle (solid or bordered, optional texture)
	else if (shapeType == 1)
	{
		float maxX = 1.0 - v_borderThickness;
		float minX = v_borderThickness;
		float maxY = 1.0 - v_borderThickness;
		float minY = v_borderThickness;

		if (v_textureCoord.x <= maxX && v_textureCoord.x >= minX && v_textureCoord.y <= maxY && v_textureCoord.y >= minY)
		{
			vec4 texColor = (texIndex >= 0 && texIndex < 32) ? texture(u_textures[texIndex], v_textureCoord) : vec4(1.0);
			o_color = v_color * texColor;
		}
		else
		{
			o_color = v_secondaryColor;
		}
	}
	// Shape Type 2: Circle (SDF anti-aliased circle or ring with border)
	else if (shapeType == 2)
	{
		float fade = 0.005;
		float innerRadius = 1.0 - v_innerRadius;
		float dist = 1.0 - length(v_localPosition);

		float circle = smoothstep(0.0, fade, dist);
		circle *= smoothstep(innerRadius + fade, innerRadius, dist);

		if (circle == 0.0)
			discard;

		float border = length(v_localPosition) - (1.0 / max(v_radius, 0.0001));
		if (border >= 1.0 - v_borderThickness)
		{
			o_color = v_secondaryColor;
		}
		else
		{
			vec4 texColor = (texIndex >= 0 && texIndex < 32) ? texture(u_textures[texIndex], v_textureCoord) : vec4(1.0);
			o_color = v_color * texColor;
			o_color.a *= circle;
		}
	}
	// Shape Type 3: Font Glyph
	else if (shapeType == 3)
	{
		float alpha = (texIndex >= 0 && texIndex < 32) ? texture(u_textures[texIndex], v_textureCoord).r : 1.0;
		o_color = v_color * vec4(1.0, 1.0, 1.0, alpha);
		if (o_color.a == 0.0)
			discard;
	}
	else
	{
		o_color = v_color;
	}
}
