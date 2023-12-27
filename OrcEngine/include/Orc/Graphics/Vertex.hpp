#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Color.hpp"

namespace orc {

struct SpriteVertex
{
	Vector4f color;
	
	Vector2f position;
	Vector2f textureCoords;

	float textureIndex;
};

struct RectangleVertex
{
	Vector4f fillColor;
	Vector4f borderColor;
	
	Vector2f position;
	Vector2f textureCoords;

	float textureIndex;
	float borderThickness;
};

struct CircleVertex
{
	Vector4f fillColor;
	Vector4f borderColor;
	
	Vector2f localPosition;
	Vector2f globalPosition;
	Vector2f textureCoords;

	float textureIndex;
	
	float radius;
	float innerRadius;
	float borderThickness;
};

struct TextVertex
{
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	Vector2f position;
	Vector2f textureCoords;

	float textureIndex;
};

struct LineVertex
{
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector2f position;
};

}
