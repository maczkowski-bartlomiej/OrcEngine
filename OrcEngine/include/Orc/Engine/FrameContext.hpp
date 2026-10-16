#pragma once

namespace orc {

class Input;
struct LayerContext;

struct FrameContext
{
	float deltaTime = 0.0f;
	const Input& input;
	LayerContext& context;
};

}
