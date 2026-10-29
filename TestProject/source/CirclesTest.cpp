#include "CirclesTest.hpp"

#include "DemoUtilities.hpp"

CirclesTest::CirclesTest(orc::LayerContext& layerContext)
	: orc::GameLayer(layerContext)
{
	orc::Ref<orc::Texture> gradientTexture = m_layerContext.textureResources.getResource("gradient_texture");
	orc::Ref<orc::Font> font = m_layerContext.fontResources.getResource("arial_font");

	m_items.reserve(11);

	{
		orc::Circle shape(orc::Vector2f(100.0f, 75.0f), 35.0f, orc::Color(0, 0, 255));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Blue Color", orc::Vector2f(100.0f, 20.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(300.0f, 75.0f), 35.0f);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured", orc::Vector2f(300.0f, 20.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(500.0f, 75.0f), 35.0f, orc::Color(0, 255, 0));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured + Green Color", orc::Vector2f(500.0f, 20.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(700.0f, 75.0f), -35.0f, orc::Color(255, 255, 255));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "White + Negative Radius", orc::Vector2f(700.0f, 20.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(100.0f, 225.0f), 35.0f, orc::Color(255, 255, 255, 255));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setInnerRadius(0.3f);
		shape.setTexture(gradientTexture);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured + Inner Radius", orc::Vector2f(100.0f, 170.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(300.0f, 225.0f), 35.0f, orc::Color(255, 0, 0));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setInnerRadius(2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Red + Inner Radius Max", orc::Vector2f(300.0f, 170.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(500.0f, 225.0f), 35.0f, orc::Color(255, 0, 0));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setInnerRadius(-0.2f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Red + Negative Inner Radius", orc::Vector2f(500.0f, 170.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(700.0f, 225.0f), 35.0f, orc::Color(0, 255, 0));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setTexture(gradientTexture);
		shape.setBorderThickness(0.2f);
		shape.setBorderColor(orc::Color(255, 0, 0));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Green + Textured + Red Border", orc::Vector2f(700.0f, 170.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(100.0f, 375.0f), 35.0f, orc::Color(0, 255, 0));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setBorderThickness(20.0f);
		shape.setBorderColor(orc::Color(255, 0, 0));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Green + Max Red border", orc::Vector2f(100.0f, 320.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(300.0f, 375.0f), 35.0f, orc::Color(0, 255, 0));
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		shape.setBorderThickness(-0.2f);
		shape.setBorderColor(orc::Color(255, 0, 0));
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Green + Negative Red border", orc::Vector2f(300.0f, 320.0f))});
	}

	{
		orc::Circle shape(orc::Vector2f(500.0f, 375.0f), 35.0f, orc::Color(255, 255, 255));
		shape.setBorderThickness(0.05f);
		shape.setTexture(gradientTexture);
		shape.setOrigin(shape.getGlobalRect().getSize() / 2.0f);
		m_items.push_back(GridItem{
			.shape = shape,
			.label = demo::createShowcaseLabel(font, "Textured + Border + Rotating", orc::Vector2f(500.0f, 320.0f))});
	}

	m_circle11 = &m_items[10].shape;
}

CirclesTest::~CirclesTest()
{
}

void CirclesTest::onAttach()
{
	ORC_LOG_INFO("Switching to CirclesTest.");
	m_layerContext.window.setTitle("CirclesTest");
}

void CirclesTest::onDetach()
{
}

void CirclesTest::onUpdate(float deltaTime)
{
	demo::handleLayerNavigation(m_layerContext, "CameraTest", "InputTest");

	m_circle11->rotate(45.0f * deltaTime);
}

void CirclesTest::onRender()
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

void CirclesTest::onGuiRender()
{
	demo::renderNavigationMenu(m_layerContext.gameLayerManager);
}
