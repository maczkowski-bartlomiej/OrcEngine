#include "AnimationTest.hpp"

#include "DemoUtilities.hpp"

AnimationTest::AnimationTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
}

AnimationTest::~AnimationTest()
{
}

void AnimationTest::onAttach()
{
	ORC_LOG_INFO("Switching to AnimationTest");
	m_layerContext.window.setTitle("AnimationTest");
}

void AnimationTest::onDetach()
{
}

void AnimationTest::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "SpritesTest", "AudioTest");
}

void AnimationTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));

	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.endScene();
}

void AnimationTest::onGuiRender()
{
	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
