#pragma once

#include "VertexArray.hpp"

#include "Math/Vector4.hpp"
#include "System/System.hpp"

namespace orc {

class RenderCommand
{
public:
	static void clear();
	static void setClearColor(const Vector4f& color);

	static void drawIndexed(Reference<VertexArray> vertexArray);
};

}
