#include "RectanglesTest.hpp"

#include "DemoUtilities.hpp"

RectanglesTest::RectanglesTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	orc::Ref<orc::Texture> gradientTexture = m_layerContext.textureResources.getResource("gradient_texture");
	orc::Ref<orc::Font> font = m_layerContext.fontResources.getResource("arial_font");

	m_items.reserve(12);

	{
		orc::Rectangle shape(orc::Vector2f(100.0f, 75.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 0, 255));
		shape.setOrigin(shape.getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Blue Color", orc::Vector2f(100.0f, 20.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(300.0f, 75.0f), orc::Vector2f(75.0f, 75.0f));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured", orc::Vector2f(300.0f, 20.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(500.0f, 75.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 255, 0));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured + Green Color", orc::Vector2f(500.0f, 20.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(700.0f, 75.0f), orc::Vector2f(75.0f, 75.0f));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured + Rotating", orc::Vector2f(700.0f, 20.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(100.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 0, 255, 90));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setBorderThickness(0.05f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Blue Opacity + Default Border", orc::Vector2f(100.0f, 170.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(300.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 0, 0));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setBorderThickness(0.05f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Black + Negative Border", orc::Vector2f(300.0f, 170.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(500.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 255, 255));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		shape.setBorderThickness(0.05f);
		shape.setBorderColor(orc::Color(255, 0, 0));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured + Red Border", orc::Vector2f(500.0f, 170.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(700.0f, 225.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 255, 0));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		shape.setBorderThickness(0.2f);
		shape.setBorderColor(orc::Color(0, 0, 255));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Green + Textured + Blue Border", orc::Vector2f(700.0f, 170.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(100.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(0, 255, 0));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setBorderThickness(20.0f);
		shape.setBorderColor(orc::Color(255, 0, 0));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Green + Red Border Max", orc::Vector2f(100.0f, 320.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(300.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 0, 255));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setBorderThickness(0.05f);
		shape.setRotation(45.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Pink + Border + 45 degrees", orc::Vector2f(300.0f, 320.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(500.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 255, 0));
		shape.setBorderThickness(0.05f);
		shape.setScale(orc::Vector2f(0.5f, 0.5f));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Yellow + Border + 0.5 scale", orc::Vector2f(500.0f, 320.0f))});
	}

	{
		orc::Rectangle shape(orc::Vector2f(700.0f, 375.0f), orc::Vector2f(75.0f, 75.0f), orc::Color(255, 0, 0));
		shape.setScale(orc::Vector2f(-0.5f, -0.5f));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Red + 0.5 Negative Scaled", orc::Vector2f(700.0f, 320.0f))});
	}

	m_rectangle4 = &m_items[3].shape;
}

RectanglesTest::~RectanglesTest()
{
}

void RectanglesTest::onAttach()
{
	ORC_LOG_INFO("Switching to RectanglesTest.");
	m_layerContext.window.setTitle("RectanglesTest");
}

void RectanglesTest::onDetach()
{
}

void RectanglesTest::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "Menu", "SpritesTest");

	m_rectangle4->rotate(65.0f * deltaTime);
}

void RectanglesTest::onRender()
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

void RectanglesTest::onGuiRender()
{
	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
