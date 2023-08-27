#pragma once

#include "Engine/Core.hpp"

namespace orc {

struct Vertex
{
	Vector2f position;
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
};

struct TexturedVertex
{
	Vector2f position;
	Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector2f textureCoords;
	float textureIndex = 0.0f;
};

}
