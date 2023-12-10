#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Color.hpp"

namespace orc {

struct Vertex
{
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2f position;

	Vector2f textureCoords;
	float textureIndex;
};

struct RectangleVertex
{
	Vector4f fillColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4f borderColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2f position;

	Vector2f textureCoords;
	float textureIndex;

	float borderThickness = 0.01f;
};

struct CircleVertex
{
	Vector4f fillColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4f borderColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2f localPosition;
	Vector2f globalPosition;

	Vector2f textureCoords;
	float textureIndex;

	float radius = 1.0f;
	float thickness = 1.0f;
	float borderThickness = 0.05f;
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
