#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Color.hpp"

namespace orc {

struct SpriteVertex
{
	Vector4f color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	Vector2f position;
	Vector2f textureCoord;

	float textureIndex;
};

struct RectangleVertex
{
	Vector4f fillColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4f borderColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	Vector2f position;
	Vector2f textureCoord;

	float textureIndex;
	float borderThickness = 0.0f;
};

struct CircleVertex
{
	Vector4f fillColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4f borderColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	Vector2f localPosition;
	Vector2f globalPosition;
	Vector2f textureCoord;

	float textureIndex;
	
	float radius = 0.0f;
	float innerRadius = 0.0f;
	float borderThickness = 0.0f;
};

struct GlyphVertex
{
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	Vector2f position;
	Vector2f textureCoord;

	float textureIndex;
};

struct LineVertex
{
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Vector2f position;
};

}
