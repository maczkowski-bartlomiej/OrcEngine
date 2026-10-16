#include "ScriptTest.hpp"

ScriptTest::ScriptTest(orc::LayerContext& context)
	: orc::GameLayer(context)
{
}

ScriptTest::~ScriptTest()
{
}

void ScriptTest::onAttach()
{
}

void ScriptTest::onDetach()
{
}

void ScriptTest::onUpdate(const orc::FrameContext& ctx)
{
}

void ScriptTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);
}

void ScriptTest::onRender(orc::Renderer& renderer)
{
}

void ScriptTest::onGuiRender()
{
}
