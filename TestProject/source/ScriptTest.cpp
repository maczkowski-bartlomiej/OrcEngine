#include "ScriptTest.hpp"

#include "DemoUtilities.hpp"

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
	demo::handleLayerNavigation(m_layerContext, "Menu", "Menu");
}

void ScriptTest::onRender()
{
}

void ScriptTest::onGuiRender()
{
	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
