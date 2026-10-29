#include "SpritesTest.hpp"

#include "DemoUtilities.hpp"

SpritesTest::SpritesTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	orc::Ref<orc::Texture> zombieTexture = m_layerContext.textureResources.getResource("zombie_texture");
	orc::Ref<orc::Texture> soldierTexture = m_layerContext.textureResources.getResource("soldier_texture");
	orc::Ref<orc::Font> font = m_layerContext.fontResources.getResource("arial_font");

	m_items.reserve(6);

	{
		orc::Sprite shape(soldierTexture, orc::Vector2f(100.0f, 75.0f));
		shape.setScale(0.5f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Soldier Texture", orc::Vector2f(100.0f, 20.0f))});
	}

	{
		orc::Sprite shape(zombieTexture, orc::Vector2f(300.0f, 75.0f));
		shape.setScale(0.5f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Zombie Texture", orc::Vector2f(300.0f, 20.0f))});
	}

	{
		orc::Sprite shape(soldierTexture, orc::Vector2f(500.0f, 75.0f));
		shape.setScale(0.5f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setRotation(45.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Soldier + Rotated 45", orc::Vector2f(500.0f, 20.0f))});
	}

	{
		orc::Sprite shape(soldierTexture, orc::Vector2f(700.0f, 75.0f));
		shape.setScale(0.5f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Soldier + Rotating", orc::Vector2f(700.0f, 20.0f))});
	}

	{
		orc::Sprite shape(soldierTexture, orc::Vector2f(100.0f, 225.0f));
		shape.setScale(0.5f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setColor(orc::Color(255, 0, 0));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Soldier + Red color", orc::Vector2f(100.0f, 170.0f))});
	}

	{
		orc::Sprite shape(zombieTexture, orc::Vector2f(300.0f, 225.0f));
		shape.setScale(-0.5f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Zombie + negative 0.5 scaled", orc::Vector2f(300.0f, 170.0f))});
	}

	m_sprite4 = &m_items[3].shape;
}

SpritesTest::~SpritesTest()
{
}

void SpritesTest::onAttach()
{
	ORC_LOG_INFO("Switching to SpritesTest.");
	m_layerContext.window.setTitle("SpritesTest");
}

void SpritesTest::onDetach()
{
}

void SpritesTest::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "RectanglesTest", "AnimationTest");

	m_sprite4->rotate(45.0f * deltaTime);
}

void SpritesTest::onRender()
{
	m_layerContext.renderer.setClearColor(orc::Color(25, 25, 25, 255));

	m_layerContext.renderer.beginScene(m_camera);

	for (const auto& item : m_items)
	{
		m_layerContext.renderer.draw(item.shape);
		m_layerContext.renderer.draw(item.label);
	}

	demo::drawGrid(m_layerContext.renderer);

	m_layerContext.renderer.endScene();
}

void SpritesTest::onGuiRender()
{
	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
