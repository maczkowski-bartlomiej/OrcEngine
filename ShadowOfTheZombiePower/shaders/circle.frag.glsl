#version 460 core

in vec4 v_fillColor;
in vec4 v_borderColor;
in vec2 v_localPosition;
in vec2 v_textureCoords;
in float v_textureIndex;
in float v_radius;
in float v_innerRadius;
in float v_borderThickness;

uniform sampler2D u_textures[32];

out vec4 color;

void main()
{
    float fade = 0.0005;
    float innerRadius = 1.0 - v_innerRadius;

    float distance = 1.0 - length(v_localPosition);
    float circle = smoothstep(0.0, fade, distance);
    circle *= smoothstep(innerRadius + fade, innerRadius, distance);
 
    if (circle == 0.0f)
        discard;
    
    float border = length(v_localPosition) - (1 / v_radius);
    if (border >= 1.0 - v_borderThickness)
    {
        color = v_borderColor;
    }
    else
    {
        color = v_fillColor;
        color.a *= circle;

        int textureIndex = int(v_textureIndex);
        if (textureIndex != 32)
        {
            color *= texture(u_textures[int(v_textureIndex)], v_textureCoords);
        }

    }
}
