#version 460

in vec4 v_fillColor;
in vec4 v_borderColor;
in vec2 v_textureCoords;
in float v_textureIndex;
in float v_borderThickness;

uniform sampler2D u_textures[32];

out vec4 color;

void main()
{
    float maxX = 1.0 - v_borderThickness;
    float minX = v_borderThickness;
    float maxY = maxX / 1.0;
    float minY = minX / 1.0;

    if (v_textureCoords.x < maxX && v_textureCoords.x > minX && v_textureCoords.y < maxY && v_textureCoords.y > minY)
    {
        color = v_fillColor;
        int textureIndex = int(v_textureIndex);
        if (textureIndex != 32)
        {
            color *= texture(u_textures[int(v_textureIndex)], v_textureCoords);
        }
    }
    else
    {
        color = v_borderColor;
    }
}
