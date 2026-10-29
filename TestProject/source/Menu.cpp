#include "Menu.hpp"

#include "DemoUtilities.hpp"

#include "AnimationTest.hpp"
#include "AudioTest.hpp"
#include "CameraTest.hpp"
#include "CirclesTest.hpp"
#include "InputTest.hpp"
#include "RectanglesTest.hpp"
#include "SpritesTest.hpp"

Menu::Menu(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	m_layerContext.gameLayerManager.addLayer("AnimationTest", orc::createRef<AnimationTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("AudioTest", orc::createRef<AudioTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("CameraTest", orc::createRef<CameraTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("CirclesTest", orc::createRef<CirclesTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("InputTest", orc::createRef<InputTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("RectanglesTest", orc::createRef<RectanglesTest>(m_layerContext));
	m_layerContext.gameLayerManager.addLayer("SpritesTest", orc::createRef<SpritesTest>(m_layerContext));
}

Menu::~Menu()
{
}

void Menu::onAttach()
{
	ORC_LOG_INFO("Switching to Menu.");
	m_layerContext.window.setTitle("Menu");
}

void Menu::onDetach()
{
}

void Menu::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "InputTest", "RectanglesTest");
}

void Menu::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));
	m_layerContext.renderer.beginScene(m_camera);
	m_layerContext.renderer.endScene();
}

void Menu::onGuiRender()
{
	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
