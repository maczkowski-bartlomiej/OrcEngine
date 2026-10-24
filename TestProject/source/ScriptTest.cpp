#include "ScriptTest.hpp"

ScriptTest::ScriptTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
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

void ScriptTest::onUpdate(float deltaTime)
{
}

void ScriptTest::onEvent(const orc::Event& event)
{
	orc::GameLayer::onEvent(event);
}

void ScriptTest::onRender()
{
}

void ScriptTest::onGuiRender()
{
}
