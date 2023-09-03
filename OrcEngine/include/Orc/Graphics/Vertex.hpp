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

struct CircleVertex
{
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2f textureCoords;
	Vector2f localPosition;
	Vector2f globalPosition;

	float fade;
	float thickness;
	float textureIndex;
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
