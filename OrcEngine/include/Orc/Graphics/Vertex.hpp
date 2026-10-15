#pragma once

#include "Engine/Core.hpp"
#include "Graphics/Color.hpp"

namespace orc {

struct Vertex
{
	Vector3f position;
	Vector4f color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4f secondaryColor = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	Vector2f textureCoord;
	Vector2f localPosition;

	float textureIndex = 32.0f;
	float shapeType = 0.0f;
	float borderThickness = 0.0f;
	float innerRadius = 0.0f;
	float radius = 0.0f;
};

}
